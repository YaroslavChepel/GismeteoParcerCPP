#include "getcitytemp.h"

getCityTemp::getCityTemp()
{

}
void getCityTemp::getTempForCity(QString link) {
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request(QUrl("https://www.gismeteo.com/"+link+"/now/"));
    QNetworkReply *reply = manager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        QStringList infoList;
        if(reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QString htmlContent = QString::fromUtf8(data);
            html::parser p;
            html::node_ptr node = p.parse(htmlContent.toStdString());

            node->walk([&infoList](html::node& n) {
                if(n.type_node == html::node_t::tag && n.tag_name == "span") {
                    infoList.append(QString::fromStdString(n.to_html()));
                }
                return true; // scan child tags
            });
            html::node_ptr root = p.parse(infoList.first().toStdString());
            int running = 0;
            QString finalTemp;
            root->walk([&running, &finalTemp](html::node& n) {
                if(n.type_node == html::node_t::tag && n.tag_name == "span" && running == 0) {
                    finalTemp = QString::fromStdString(n.to_text());
                    running =1;
                }

                return true; // scan child tags
            });
            m_currentTemp.append(finalTemp);
            std::vector<html::node*> weatherNow = node->select(".now-desc");
            for (const auto& node : weatherNow) {
                m_currentTemp.append(QString::fromStdString(node->to_text()));
            }
            std::vector<html::node*> timeNow = node->select(".now-localdate");
            for (const auto& node : timeNow) {
                m_currentTemp.append(QString::fromStdString(node->to_text()));
            }
            emit currentTempChanged();
        } else {
            qDebug () << "Failed to connect to gismeteo.com";
        }
        reply->deleteLater();
        emit getSucsessful();
    });
}
void getCityTemp::gismeteoSearch(QString searchQuery) {
    QNetworkAccessManager *manager = new QNetworkAccessManager();;
    QNetworkRequest request(QUrl("https://www.gismeteo.com/search/" + searchQuery));
    QNetworkReply *reply = manager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if(reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QString htmlContent = QString::fromUtf8(data);
            html::parser p;
            html::node_ptr node = p.parse(htmlContent.toStdString());
            std::vector<html::node*> selectedNodes = node->select(".catalog-item");
            for (const auto& node : selectedNodes) {
                std::string combinedText;

                std::vector<html::node*> aElements = node->select("a");
                for (const auto& aElement : aElements) {
                    std::string text = aElement->to_text();

                    QString clearedText = QString::fromStdString(text).replace(QRegularExpression("[^\\p{L}\\s-]"), "");

                    if (!clearedText.isEmpty() && clearedText.at(clearedText.length() - 1) == QChar::Space) {
                        clearedText.chop(1);
                    }
                    combinedText += clearedText.toStdString() +"%"+ aElement->get_attr("href") + ", ";
                }

                combinedText = combinedText.substr(0, combinedText.length()); // Remove trailing space
                m_searchResult.append(QString::fromStdString(combinedText));
                }
            emit searchResultChanged();

        } else {
            qDebug () << "Failed to connect to gismeteo.com";
        }
        reply->deleteLater();
        emit searchFinished();
    });
}
QStringList getCityTemp::searchResult() const {
    return m_searchResult;
}
QStringList getCityTemp::currentTemp() const {
    return m_currentTemp;
}

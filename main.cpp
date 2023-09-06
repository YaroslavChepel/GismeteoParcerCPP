#include <QCoreApplication>
#include <iostream>
#include <html.hpp>
#include <QStringList>
#include <cstdio>
#include <getcitytemp.h>
#include <QStringList>
#include <QTextStream>
#include <windows.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    getCityTemp getTemp;
    QString searchQuery;
    QTextStream output(stdout);
    QTextStream input(stdin);
    int selectedValue;
    SetConsoleOutputCP( 65001 );

    std::cout << "Welcome to GismeteoParcer, enter a city name you would like to get weather:\n";
    input >> searchQuery;
    input.flush();
    getTemp.gismeteoSearch(searchQuery);
    QObject::connect(&getTemp,&getCityTemp::getSucsessful, [&a, &selectedValue, &getTemp] (){
        std::cout << "-----------------------\n" + getTemp.searchResult().at(selectedValue-1).split(", ")[0].split("%")[0].toStdString() +"\n\n"
                         + "Date: "
                         + getTemp.currentTemp().at(2).toStdString()
                         + "\nTemperature now: "
                         + getTemp.currentTemp().at(0).toStdString()
                         + "\nWeather now: "
                         + getTemp.currentTemp().at(1).toStdString()
                         + "\n-----------------------";
        std::cout << "\nPress Enter key to exit...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getchar();
        a.quit();
    });
    QObject::connect(&getTemp, &getCityTemp::searchFinished, [&getTemp, &output, &a, &selectedValue]() {
        int elemNow = 1;

        if (getTemp.searchResult().empty()) {
            output << "Nothing found!";
            output.flush();
            std::cout << "\nPress Enter key to exit...";
            std::getchar();
            a.quit();
        } else {
            if (getTemp.searchResult().length() <= 10) {
                foreach (const QString &elem, getTemp.searchResult()) {
                    output << elemNow << ". " << elem.split(", ")[0].split("%")[0]
                           << " (" << elem.split(", ")[1].split("%")[0]
                           << ", " << elem.split(", ")[2].split("%")[0]
                           << ", " << elem.split(", ")[3].split("%")[0]<<")" << "\n";
                    output.flush();
                    elemNow +=1;
                }
            } else {
                foreach (const QString &elem, getTemp.searchResult().sliced(0,10)) {
                    output << elemNow << ". " << elem.split(", ")[0].split("%")[0]
                           << " (" << elem.split(", ")[1].split("%")[0]
                           << ", " << elem.split(", ")[2].split("%")[0]
                           << ", " << elem.split(", ")[3].split("%")[0]<<")" << "\n";
                    output.flush();
                    elemNow +=1;
                }
            }
            output << "Now choose a correct city number:\n";
            output.flush();
            std::cin >> selectedValue;
            output << "Great! Now searching for: " << getTemp.searchResult().at(selectedValue-1).split(", ")[0].split("%")[0] << "\n";
            output.flush();
            getTemp.getTempForCity(getTemp.searchResult().at(selectedValue-1).split(", ")[0].split("%")[1]);
        }
    });
    return a.exec();
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPushButton>
#include <QMap>
#include <QLabel>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTimer>  // Include QTimer
#include <QTime>   // Include QTime

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void updateLabelsForAllBuses(const QString &fromLocation, const QString &toLocation , const QString &date);

    ~MainWindow();

private slots:


    void on_btnLogin_clicked();

    void on_btnCancel_clicked();

    void on_btnSignUp_clicked();

    void on_pushButton_find_schedule_clicked();

    void on_btnSignUp2_clicked();

    void on_btnGoBack_clicked();

    void on_btnGoBack_2_clicked();

    void on_btnGoBack_3_clicked();

    void on_btnGoBack_4_clicked();

    void on_btnGoBack_5_clicked();

    void onSeatButtonClicked();

    void onGenderSelected(int seatIndex, bool male);

    void onBusButtonClicked(QAbstractButton* button);

    void on_btnConfirmBooking_clicked();

    void loadBusSchedule();

    void saveBookingToFile(const QString& username, const QString& from, const QString& to, const QString& date, const QString& busNumber, const QString& seatNumber, const QString& price, const QString& selectedGender);

    void updateSeatColors(const QString& from, const QString& to, const QString& busNumber, const QString& date);


    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void updateClock();  // Slot for updating the real-time clock

    void on_btnAnotherTicket_clicked();

    void on_btnExit_clicked();

    void on_toolButton_2_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    void createScheduleFile(); // Declare the function here
    void updateScheduleFromFile(const QString &fromLocation, const QString &toLocation, const QString &date);
    QList<QPushButton*> seatButtons; // Store all seat buttons
    QPushButton* clickedButton;      // The button that was clicked
    QMap<int, QString> seatStates;
    int currentlySelectedSeatIndex = -1;
    QString selectedSeatNumber;
    QString busNumber;          // Stores the selected bus number
    QButtonGroup *busButtonGroup; // Group for bus buttons
    QString selectedFrom;   // Store selected "From" location
    QString selectedTo;     // Store selected "To" location
    QString selectedPrice;  // Store the price for the selected route
    QString selectedTime;   // Store the time for the selected route
    QString selectedDistance; // Store the distance for the selected route
    QString selectedGender;            // Gender of the current booking (male/female)
};
#endif // MAINWINDOW_H

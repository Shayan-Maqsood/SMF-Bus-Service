#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QString"
#include "QMessageBox"
#include "QPixmap"
#include <QFile>
#include <QVBoxLayout>
#include <QTextStream>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QList>
#include <QButtonGroup>
#include <QLayout>
#include <QRegularExpression>
#include <QDate>
#include <QTimer>  // Include QTimer
#include <QTime>   // Include QTime

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->txtName->setPlaceholderText("Username");
    ui->txtPass->setPlaceholderText("Password");
    ui->txtFirstName->setPlaceholderText("First Name");
    ui->txtLastName->setPlaceholderText("Last Name");
    ui->txtEmail->setPlaceholderText("Email");
    ui->txtPhone->setPlaceholderText("Phone Number");
    ui->txtPassSignUp->setPlaceholderText("Password");
    ui->txtCfrmPassSignUp->setPlaceholderText(" Confirm Password");

    // Initialize the button group
    busButtonGroup = new QButtonGroup(this);

    // Add bus buttons to the group
    busButtonGroup->addButton(ui->btnBus1, 1);
    busButtonGroup->addButton(ui->btnBus2, 2);
    busButtonGroup->addButton(ui->btnBus3, 3);
    busButtonGroup->addButton(ui->btnBus4, 4);
    busButtonGroup->addButton(ui->btnBus5, 5);
    busButtonGroup->addButton(ui->btnBus6, 6);
    busButtonGroup->addButton(ui->btnBus7, 7);
    busButtonGroup->addButton(ui->btnBus8, 8);
    busButtonGroup->addButton(ui->btnBus9, 9);
    busButtonGroup->addButton(ui->btnBus10, 10);
    busButtonGroup->addButton(ui->btnBus11, 11);
    busButtonGroup->addButton(ui->btnBus12, 12);
    busButtonGroup->addButton(ui->btnBus13, 12);
    busButtonGroup->addButton(ui->btnBus14, 14);
    busButtonGroup->addButton(ui->btnBus15, 15);


    // Connect the button group's signal to a slot
    connect(busButtonGroup, &QButtonGroup::buttonClicked, this, &MainWindow::onBusButtonClicked);



    // Create seat buttons dynamically (for example, 45 seats)
    seatButtons = {
                   ui->btnSeat1, ui->btnSeat2, ui->btnSeat3, ui->btnSeat4, ui->btnSeat5,
                   ui->btnSeat6, ui->btnSeat7, ui->btnSeat8, ui->btnSeat9, ui->btnSeat10,
                   ui->btnSeat11, ui->btnSeat12, ui->btnSeat13, ui->btnSeat14, ui->btnSeat15,
                   ui->btnSeat16, ui->btnSeat17, ui->btnSeat18, ui->btnSeat19, ui->btnSeat20,
                   ui->btnSeat21, ui->btnSeat22, ui->btnSeat23, ui->btnSeat24, ui->btnSeat25,
                   ui->btnSeat26, ui->btnSeat27, ui->btnSeat28, ui->btnSeat29, ui->btnSeat30,
                   ui->btnSeat31, ui->btnSeat32, ui->btnSeat33, ui->btnSeat34, ui->btnSeat35,
                   ui->btnSeat36, ui->btnSeat37, ui->btnSeat38, ui->btnSeat39, ui->btnSeat40,
                   ui->btnSeat41, ui->btnSeat42, ui->btnSeat43, ui->btnSeat44, ui->btnSeat45,
        ui->btnSeat46, ui->btnSeat47, ui->btnSeat48, ui->btnSeat49
    };

    // Initialize the seat states (all unselected)
    for (int i = 0; i < seatButtons.size(); ++i) {
        seatStates[i] = "unselected";  // Seat is initially unselected
    }

    // Connect each seat button to the same slot
    for (QPushButton* button : seatButtons) {
        connect(button, &QPushButton::clicked, this, &MainWindow::onSeatButtonClicked);
    }
    QPixmap search(":/img/img/search-interface-symbol.png");
    int w = ui->label_search->width();
    int h = ui->label_search->height();
    ui->label_search->setPixmap(search.scaled(w,h,Qt::KeepAspectRatio));

    ui->comboBox_from->setPlaceholderText("Select Departure");
    ui->comboBox_to->setPlaceholderText("Select Destination");
    ui->comboBox_date->setPlaceholderText("Select Date");
    ui->comboBox_from->addItems({"LAHORE","RAWALPINDI(MAIN)","FAISALABAD"});
    ui->comboBox_to->addItems({"LAHORE","RAWALPINDI(MAIN)","FAISALABAD"});
    ui->busNumberBox->addItems({"bus1","bus2","bus3","bus4","bus5","bus6","bus7","bus8","bus9","bus10","bus11","bus12","bus13","bus14","bus15"});
    ui->seatNumberBox->addItems({"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40","41","42","43","44","45","46","47","49"});

    QDate currentDate = QDate::currentDate();
    QStringList dateList;
    for (int i = 0; i < 5; ++i) {
        QDate nextDate = currentDate.addDays(i);
        QString formattedDate = nextDate.toString("ddd, dd MMM yyyy");
        dateList << formattedDate;
    }
    ui->comboBox_date->addItems(dateList);

    ui->fromBox->setPlaceholderText("Select Departure");
    ui->toBox->setPlaceholderText("Select Destination");
    ui->dateBox->setPlaceholderText("Select Date");
    ui->fromBox->addItems({"LAHORE","RAWALPINDI(MAIN)","FAISALABAD"});
    ui->toBox->addItems({"LAHORE","RAWALPINDI(MAIN)","FAISALABAD"});
    QDate currentDate2 = QDate::currentDate();
    QStringList dateList2;
    for (int i = 0; i < 5; ++i) {
        QDate nextDate = currentDate.addDays(i);
        QString formattedDate = nextDate.toString("ddd, dd MMM yyyy");
        dateList << formattedDate;
    }
    ui->dateBox->addItems(dateList);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadBusSchedule()
{
    QFile file("bus_schedule.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Unable to load bus schedule file.");
        return;
    } else {
        qDebug() << "File opened successfully";
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        qDebug() << "Reading line: " << line;

        // Define the regex to match the data
        QRegularExpression regex(
            R"(From:\s*(.*?)\s*To:\s*(.*?)\s*Price:\s*(\d+)\s*Seats:\s*(\d+)\s*Distance:\s*(.*?)\s*Time:\s*(.*))");

        QRegularExpressionMatch match = regex.match(line);
        if (match.hasMatch()) {
            QString from = match.captured(1).trimmed();
            QString to = match.captured(2).trimmed();
            QString price = match.captured(3).trimmed();
            QString seats = match.captured(4).trimmed();
            QString distance = match.captured(5).trimmed();
            QString time = match.captured(6).trimmed();

            // Store the values in variables (for example, when a route is selected)
            qDebug() << "From: " << from << ", To: " << to << ", Price: " << price << ", Distance: " << distance << ", Time: " << time;

            // Store these values in global variables or map them based on the route (from-to)
            if (from == selectedFrom && to == selectedTo) {
                // If this route matches the selected route
                selectedPrice = price;
                selectedDistance = distance;
                selectedTime = time;

                qDebug() << "Selected Price: " << selectedPrice << ", Distance: " << selectedDistance << ", Time: " << selectedTime;
                break;  // Found the route, no need to keep reading the file
            }
        }
    }

    file.close();
}



void MainWindow::on_btnLogin_clicked()
{
    QString enteredFullName = ui->txtName->text();
    QString enteredPassword = ui->txtPass->text();

    if (enteredFullName.isEmpty() || enteredPassword.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both your full name and password.");
        return;
    }
    else{
        QFile file("user_data.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            bool loginSuccess = false;

            while (!in.atEnd()) {
                QString line = in.readLine();
                QStringList userData = line.split(',');

                // Check if the entered username and password match any entry
                if (userData.size() == 2) {
                    QString savedFullName = userData[0];
                    QString savedPassword = userData[1];

                    if (enteredFullName == savedFullName && enteredPassword == savedPassword) {
                        loginSuccess = true;
                        break;
                    }
                }
            }

            file.close();

            if (loginSuccess) {
                QMessageBox::information(this, "Login Success", "Welcome, " + enteredFullName);
                ui->stackedWidget->setCurrentIndex(2);
                ui->HiLabel->setText("Welcome, " + enteredFullName);
            } else {
                QMessageBox::critical(this, "Login Failed", "Incorrect username or password.");
                ui->stackedWidget->setCurrentIndex(0);
            }
        } else {
            QMessageBox::critical(this, "Error", "Unable to open file.");
        }

    }
}

void MainWindow::on_btnCancel_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,ui->txtName->text(),"Are you sure you want to quit the application?",QMessageBox::Yes | QMessageBox::No);
    if(reply==QMessageBox::Yes){
        QApplication::quit();
    }
}


void MainWindow::on_btnSignUp_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}

void MainWindow::updateLabelsForAllBuses(const QString &fromLocation, const QString &toLocation , const QString &date)
{
    // List of all "From" and "To" labels
    QList<QLabel*> fromLabels = {ui->fromLabel, ui->fromLabel_2, ui->fromLabel_3, ui->fromLabel_4,ui->fromLabel_9, ui->fromLabel_10, ui->fromLabel_11, ui->fromLabel_13,ui->fromLabel_14, ui->fromLabel_15, ui->fromLabel_16, ui->fromLabel_17,ui->fromLabel_18, ui->fromLabel_19, ui->fromLabel_20, ui->fromLabel_21,ui->fromLabel_22};
    QList<QLabel*> toLabels = {ui->toLabel, ui->toLabel_2, ui->toLabel_3, ui->toLabel_4, ui->toLabel_9, ui->toLabel_10, ui->toLabel_11, ui->toLabel_13, ui->toLabel_14, ui->toLabel_15, ui->toLabel_16, ui->toLabel_17, ui->toLabel_18, ui->toLabel_19, ui->toLabel_20,ui->toLabel_21,ui->toLabel_22};
    QList<QLabel*> dateLabels = {ui->dateLabel,ui->dateLabel_3,ui->dateLabel_4,ui->dateLabel_5,ui->dateLabel_6,ui->dateLabel_7,ui->dateLabel_8};
    // Iterate through the labels and update them with the selected "From" and "To" locations
    for (QLabel* label : fromLabels) {
        label->setText(fromLocation);
    }

    for (QLabel* label : toLabels) {
        label->setText(toLocation);
    }
    for (QLabel* label : dateLabels) {
        label->setText(date);
    }
}


void MainWindow::on_pushButton_find_schedule_clicked()
{
    if (ui->comboBox_date->currentText().isEmpty() ||
        ui->comboBox_from->currentText().isEmpty() ||
        ui->comboBox_to->currentText().isEmpty())
    {
        // Show a warning message
        QMessageBox::warning(this, "Input Error", "Please fill all the required fields.");
        return; // Exit the function without proceeding
    }

    // Get selected values
    QString fromLocation = ui->comboBox_from->currentText();
    QString toLocation = ui->comboBox_to->currentText();
    QString date = ui->comboBox_date->currentText();

    // Update all labels for all buses
    updateLabelsForAllBuses(fromLocation, toLocation, date);

    // Read bus schedule data from file
    QFile file("C:/Users/hp/Desktop/BusReservationSystem/build/Desktop_Qt_6_8_0_MinGW_64_bit-Debug/bus_schedule.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line;
        int busIndex = 1;  // Start from bus 1

        // Read each line from the file and check if it matches the selected "From" and "To" locations
        while (!in.atEnd()) {
            line = in.readLine();
            QStringList busData = line.split(' ');  // Split by space

            if (busData.size() >= 4) {
                // Extract From, To, Price, and Seats data from the line
                QString from = busData[0].split(":")[1];  // Extract location after "From:"
                QString to = busData[1].split(":")[1];    // Extract location after "To:"
                QString price = busData[2].split(":")[1];  // Extract price after "Price:"
                QString seats = busData[3].split(":")[1];  // Extract seats after "Seats:"
                QString distance = busData[4].split(":")[1];  // Extract distnace after "Distance:"
                QString time = busData[5].split(":")[1];  // Extract time after "Time:"


                // If the From and To locations match the user's selection, update the corresponding labels
                if (from == fromLocation && to == toLocation) {
                    if (busIndex == 1) {
                        ui->price1->setText("Rs." + price);
                        ui->seats1->setText(seats + " Seats");
                        ui->d1->setText(distance);
                        ui->t1->setText(time);

                        ui->price5->setText("Rs." + price);
                        ui->seats5->setText(seats + " Seats");
                        ui->d5->setText(distance);
                        ui->t5->setText(time);

                        ui->price8->setText("Rs." + price);
                        ui->seats8->setText(seats + " Seats");
                        ui->d8->setText(distance);
                        ui->t8->setText(time);

                        ui->price12->setText("Rs." + price);
                        ui->seats12->setText(seats + " Seats");
                        ui->d12->setText(distance);
                        ui->t12->setText(time);

                        ui->price14->setText("Rs." + price);
                        ui->seats14->setText(seats + " Seats");
                        ui->d14->setText(distance);
                        ui->t14->setText(time);
                    } else if (busIndex == 2) {
                        ui->price2->setText("Rs." + price);
                        ui->seats2->setText(seats + " Seats");
                        ui->d2->setText(distance);
                        ui->t2->setText(time);

                        ui->price6->setText("Rs." + price);
                        ui->seats6->setText(seats + " Seats");
                        ui->d6->setText(distance);
                        ui->t6->setText(time);

                        ui->price9->setText("Rs." + price);
                        ui->seats9->setText(seats + " Seats");
                        ui->d9->setText(distance);
                        ui->t9->setText(time);

                        ui->price13->setText("Rs." + price);
                        ui->seats13->setText(seats + " Seats");
                        ui->d13->setText(distance);
                        ui->t13->setText(time);
                    } else if (busIndex == 3) {
                        ui->price3->setText("Rs." + price);
                        ui->seats3->setText(seats + " Seats");
                        ui->d3->setText(distance);
                        ui->t3->setText(time);

                        ui->price7->setText("Rs." + price);
                        ui->seats7->setText(seats + " Seats");
                        ui->d7->setText(distance);
                        ui->t7->setText(time);

                        ui->price10->setText("Rs." + price);
                        ui->seats10->setText(seats + " Seats");
                        ui->d10->setText(distance);
                        ui->t10->setText(time);
                    } else if (busIndex == 4) {
                        ui->price4->setText("Rs." + price);
                        ui->seats4->setText(seats + " Seats");
                        ui->d4->setText(distance);
                        ui->t4->setText(time);

                        ui->price11->setText("Rs." + price);
                        ui->seats11->setText(seats + " Seats");
                        ui->d11->setText(distance);
                        ui->t11->setText(time);

                        ui->price15->setText("Rs." + price);
                        ui->seats15->setText(seats + " Seats");
                        ui->d15->setText(distance);
                        ui->t15->setText(time);
                    }
                    busIndex++;  // Move to the next bus
                    if(busIndex>4){
                        break;
                    }
                }
            }
        }
        file.close();
    } else {
        QMessageBox::critical(this, "Error", "Unable to open the bus schedule file.");
    }

    // Ask user if they are sure to proceed
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, ui->txtName->text(), "Are you sure?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Get the selected date from the dropdown
        QString selectedDate = ui->comboBox_date->currentText();

        // Get the current list of dates from the dropdown (which will be 5 consecutive dates)
        QStringList dateList;
        for (int i = 0; i < ui->comboBox_date->count(); i++) {
            dateList.append(ui->comboBox_date->itemText(i));
        }

        // Find the index of the selected date in the dropdown
        int dateIndex = dateList.indexOf(selectedDate);

        // If the date is found, change the stacked widget index accordingly
        if (dateIndex != -1) {
            // The mapping assumes you want to start at page 3 for the first date, page 4 for the second, and so on
            ui->stackedWidget->setCurrentIndex(3 + dateIndex); // 3, 4, 5, 6, 7 for each date
        } else {
            QMessageBox::critical(this, "Error", "The selected date is not in the list.");
        }

    } else {
        // If user clicks No, return to page 2
        ui->stackedWidget->setCurrentIndex(2);
    }
}




void MainWindow::on_btnSignUp2_clicked()
{
    QString firstName = ui->txtFirstName->text().trimmed();
    QString lastName = ui->txtLastName->text().trimmed();
    QString email = ui->txtEmail->text().trimmed();
    QString phoneNumber = ui->txtPhone->text().trimmed();
    QString password = ui->txtPassSignUp->text().trimmed();
    QString confirmPassword = ui->txtCfrmPassSignUp->text().trimmed();

    QDate dob = ui->dateEdit_dob->date();
    QString dateOfBirth = dob.toString("yyyy-MM-dd");

    QString fullName = firstName + " " + lastName;

    if (firstName.isEmpty() || lastName.isEmpty() || email.isEmpty() ||
        !dob.isValid() || phoneNumber.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "Password Error", "Passwords do not match.");
        return;
    }else{
        QFile file("user_data.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            // Store full name and password in a new line
            out << fullName << "," << password << "\n";
            file.close();

            QMessageBox::information(this, "Success", "Account created successfully!");
            ui->stackedWidget->setCurrentIndex(0);
        } else {
            QMessageBox::critical(this, "Error", "Unable to open file.");
        }
    }
}

void MainWindow::on_btnGoBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_btnGoBack_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_btnGoBack_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_btnGoBack_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_btnGoBack_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

// Slot for when a bus button is clicked
void MainWindow::onBusButtonClicked(QAbstractButton* button)
{
    // Retrieve the button's ID
    int id = busButtonGroup->id(button);

    // Map the ID to the bus number
    busNumber = "bus" + QString::number(id);

    // Optionally display the selected bus for debugging or user feedback
    QMessageBox::information(this, "Bus Selected", "You selected " + busNumber);

    // Retrieve user-selected route and date information
    QString from = ui->comboBox_from->currentText();
    QString to = ui->comboBox_to->currentText();
    QString date = ui->comboBox_date->currentText();

    // Update seat availability based on booking details
    updateSeatColors(from, to, busNumber, date);

    // Switch to the seat selection interface
    ui->stackedWidget->setCurrentIndex(8);
}

void MainWindow::updateSeatColors(const QString& from, const QString& to, const QString& busNumber, const QString& date)
{
    // Open the file containing booking details
    QFile file("booking_details.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Unable to open booking_details.txt");
        return;
    }

    // Reset all seats to unselected state (default color)
    for (int i = 0; i < seatButtons.size(); ++i) {
        seatButtons[i]->setStyleSheet(""); // Clear previous colors
    }

    // Read each line and extract booking details
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        // Split the line by the '|' delimiter
        QStringList fields = line.split("|");
        if (fields.size() != 8) continue; // Ensure all 8 fields are present

        QString fileUsername = fields[0].trimmed();
        QString fileFrom = fields[1].trimmed();
        QString fileTo = fields[2].trimmed();
        QString fileDate = fields[3].trimmed();
        QString fileBusNumber = fields[4].trimmed();
        QString seatNumber = fields[5].trimmed();
        QString filePrice = fields[6].trimmed();
        QString fileGender = fields[7].trimmed();

        // Check if this booking matches the current bus, route, and date
        if (fileFrom.toLower() == from.toLower() &&
            fileTo.toLower() == to.toLower() &&
            fileBusNumber.toLower() == busNumber.toLower() &&
            fileDate.toLower() == date.toLower())
        {
            int seatIndex = seatNumber.toInt() - 1; // Convert to 0-indexed position
            if (seatIndex >= 0 && seatIndex < seatButtons.size()) {
                QString color = (fileGender.toLower() == "male") ? "blue" : "pink";
                seatButtons[seatIndex]->setStyleSheet("background-color: " + color + "; color: white;");
                seatStates[seatIndex] = "reserved"; // Mark this seat as reserved
                seatButtons[seatIndex]->update(); // Force update to reflect changes
            }
        }
    }

    file.close();
}




void MainWindow::onSeatButtonClicked()
{
    // Get the button that was clicked
    clickedButton = qobject_cast<QPushButton*>(sender());
    int seatIndex = seatButtons.indexOf(clickedButton);

    // Get the seat number (button text) for easier identification
    QString seatNumber = clickedButton->text();

    // Check if this seat is already reserved
    if (seatStates[seatIndex] == "reserved") {
        QMessageBox::warning(this, "Seat Taken", "This seat is already reserved. Please select a different seat.");
        return; // Prevent selection of an already reserved seat
    }

    // If the seat is already selected, unselect it
    if (seatStates[seatIndex] == "selected") {
        clickedButton->setStyleSheet("background-color: transparent; color: white;");
        seatStates[seatIndex] = "unselected"; // Reset to unselected
        currentlySelectedSeatIndex = -1;     // No seat is selected
        return;
    }

    // Unselect the previously selected seat, if any
    if (currentlySelectedSeatIndex != -1) {
        seatButtons[currentlySelectedSeatIndex]->setStyleSheet("background-color: transparent; color: white;");
        seatStates[currentlySelectedSeatIndex] = "unselected";
    }

    // Show a message box asking for gender selection
    QMessageBox msgBox;
    msgBox.setText("Select your gender:");
    QPushButton *maleButton = msgBox.addButton("Male", QMessageBox::ActionRole);
    QPushButton *femaleButton = msgBox.addButton("Female", QMessageBox::ActionRole);
    msgBox.setDefaultButton(maleButton);
    msgBox.exec();

    if (msgBox.clickedButton() == maleButton) {
        selectedGender = "male"; // Save gender
        seatButtons[seatIndex]->setStyleSheet("background-color: blue; color: white;");
        seatStates[seatIndex] = "male";
    } else if (msgBox.clickedButton() == femaleButton) {
        selectedGender = "female"; // Save gender
        seatButtons[seatIndex]->setStyleSheet("background-color: red; color: white;");
        seatStates[seatIndex] = "female";
    }

    // Mark the seat as reserved (no further selection)
    seatStates[seatIndex] = "reserved";

    // Mark the seat as selected for the current user
    currentlySelectedSeatIndex = seatIndex;
    selectedSeatNumber = seatNumber; // Store the seat number (button text)
}




void MainWindow::saveBookingToFile(const QString& username, const QString& from, const QString& to, const QString& date, const QString& busNumber, const QString& seatNumber, const QString& price, const QString& selectedGender)
{
    // Open the file in append mode
    QFile file("booking_details.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Unable to save booking details to file.");
        return;
    }

    // Write the booking details to the file in a single line
    QTextStream out(&file);
    out << username << "|"
        << from << "|"
        << to << "|"
        << date << "|"
        << busNumber << "|"
        << seatNumber << "|"
        << price << "|"
        << selectedGender << "\n";

    file.close();
}

void MainWindow::on_btnConfirmBooking_clicked()
{
    QString username = ui->txtName->text();
    QString from = ui->comboBox_from->currentText();
    QString to = ui->comboBox_to->currentText();
    QString date = ui->comboBox_date->currentText();

    if (busNumber.isEmpty() || selectedSeatNumber.isEmpty() || selectedGender.isEmpty()) {
        QMessageBox::warning(this, "Incomplete Booking", "Please select a bus, a seat, and a gender before confirming.");
        return;
    }

    // Store selected route (from -> to)
    selectedFrom = from;
    selectedTo = to;

    // Load bus schedule and get the price, time, distance
    loadBusSchedule();

    // Check if price is available after load
    if (selectedPrice.isEmpty()) {
        QMessageBox::warning(this, "Invalid Route", "No information is available for the selected route.");
        return;
    }

    // Mark the seat as reserved in the seatStates array
    int seatIndex = seatButtons.indexOf(clickedButton); // Assuming `clickedButton` is set correctly
    seatStates[seatIndex] = "reserved";  // Mark as reserved

    // Save booking details to file
    saveBookingToFile(username, from, to, date, busNumber, selectedSeatNumber, selectedPrice, selectedGender);

    // Inform the user
    QMessageBox::information(this, "Booking Confirmed", "Your booking has been saved successfully.");

    // Open and read the booking_details.txt file
    QFile file("booking_details.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        // Read all lines into a list
        QStringList lines;
        while (!in.atEnd()) {
            lines.append(in.readLine());
        }

        // Assuming the most recent booking is the last line in the file
        QString lastBookingLine = lines.last();  // Get the last line

        // Split the line by '|' separator
        QStringList bookingDetails = lastBookingLine.split("|");

        if (bookingDetails.size() >= 8) {
            // Extract details from the booking
            QString fileName = bookingDetails[0].trimmed();  // Name
            QString fileBusNumber = bookingDetails[4].trimmed();  // Bus Number
            QString fileSeatNumber = bookingDetails[5].trimmed();  // Seat Number
            QString filePrice = bookingDetails[6].trimmed();  // Price
            QString fileGender = bookingDetails[7].trimmed();  // Gender

            // Update the UI elements with the details read from the file
            ui->nameLabel->setText(fileName);
            ui->busNumber->setText(fileBusNumber);
            ui->btnSeatNumber->setText(fileSeatNumber);
            ui->priceLabel->setText(filePrice);

            // Change the background color of the button based on gender
            if (fileGender.toLower() == "male") {
                ui->btnSeatNumber->setStyleSheet("background-color: blue;");
            } else if (fileGender.toLower() == "female") {
                ui->btnSeatNumber->setStyleSheet("background-color: pink;");
            }
        }
        else {
            QMessageBox::warning(this, "File Format Error", "The booking details are not in the expected format.");
        }
    } else {
        QMessageBox::warning(this, "File Error", "Unable to open booking details file.");
    }

    file.close();
    ui->stackedWidget->setCurrentIndex(9);
}




void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_btnAnotherTicket_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_btnExit_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_toolButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);
}


void MainWindow::on_pushButton_6_clicked()
{
        // Get the details from user input fields
        QString username = ui->nameBox->text().trimmed();
        QString from = ui->fromBox->currentText().trimmed();
        QString to = ui->toBox->currentText().trimmed();
        QString date = ui->dateBox->currentText().trimmed();
        QString busNumber = ui->busNumberBox->currentText().trimmed();
        QString seatNumber = ui->seatNumberBox->currentText().trimmed();

        // Check if any input field is empty
        if (username.isEmpty() || from.isEmpty() || to.isEmpty() || date.isEmpty() || busNumber.isEmpty() || seatNumber.isEmpty()) {
            QMessageBox::warning(this, "Incomplete Information", "Please fill in all the fields to cancel a booking.");
            return;
        }

        // Open the booking details file
        QFile file("booking_details.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "File Error", "Unable to open the booking details file.");
            return;
        }

        // Read all lines from the file
        QStringList lines;
        QTextStream in(&file);
        while (!in.atEnd()) {
            lines.append(in.readLine());
        }
        file.close();

        // Format of the line to be deleted
        QString lineToDelete = QString("%1|%2|%3|%4|%5|%6")
                                   .arg(username)
                                   .arg(from)
                                   .arg(to)
                                   .arg(date)
                                   .arg(busNumber)
                                   .arg(seatNumber);

        // Flag to track if the booking was found
        bool bookingFound = false;

        // Check for matching booking and remove it
        for (int i = 0; i < lines.size(); ++i) {
            QStringList bookingDetails = lines[i].split("|");
            if (bookingDetails.size() >= 6) { // Ensure there are at least 6 elements in the line
                QString fileUsername = bookingDetails[0].trimmed();
                QString fileFrom = bookingDetails[1].trimmed();
                QString fileTo = bookingDetails[2].trimmed();
                QString fileDate = bookingDetails[3].trimmed();
                QString fileBusNumber = bookingDetails[4].trimmed();
                QString fileSeatNumber = bookingDetails[5].trimmed();

                // Check if this line matches the user's input
                if (fileUsername == username && fileFrom == from && fileTo == to &&
                    fileDate == date && fileBusNumber == busNumber && fileSeatNumber == seatNumber)
                {
                    lines.removeAt(i);  // Remove the line from the list
                    bookingFound = true;
                    break; // Exit loop after removing the first matching line
                }
            }
        }

        // Check if the booking was found and removed
        if (!bookingFound) {
            QMessageBox::warning(this, "Cancellation Failed", "No matching booking was found. Please check your details.");
            return;
        }

        // Write the updated list of bookings back to the file
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "File Error", "Unable to update the booking details file.");
            return;
        }

        QTextStream out(&file);
        for (const QString& line : lines) {
            if (!line.trimmed().isEmpty()) { // Avoid writing empty lines
                out << line << "\n";
            }
        }
        file.close();

        // Clear the input fields
        ui->nameBox->clear();


        // Notify the user of successful cancellation
        QMessageBox::information(this, "Cancellation Successful", "The ticket has been successfully canceled.");


}


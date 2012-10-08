/*****************************************************************************
routine-generator, A routine generator app that can generate routine of a full or specified week with teacher and subject combination saved in an XML file
Copyright (C) 2012 MD: Minhazul Haque;
mdminhazulhaque@gmail.com.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define D qDebug()
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Create table
    nRow = ui->spinDPW->value();
    nColumn = ui->spinCPD->value();
    ui->table->setRowCount(nRow);
    ui->table->setColumnCount(nColumn);
    ui->table->setEditTriggers(0);
    ui->table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->table->verticalHeader()->setResizeMode(QHeaderView::Stretch);

    // Read the XML
    QFile xmlr("teachers.xml");
    if(!xmlr.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error!", "Could not find the file \"teachers.xml\"", QMessageBox::Ok);
        exit(0);
    }
    document.setContent(&xmlr);
    xmlr.close();

    // Creating a vector for random class order
    for(int i=0;i<=nColumn;i++)
    {
        cArray.push_back(i);
        tArray.push_back(i);
    }
    on_buttonReGen_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LoadTeacherInfo()
{
    QDomElement root = document.firstChildElement();
    QDomNodeList items = root.elementsByTagName("teacher");
    for(int i = 0; i < items.count(); i++)
    {
        QString teacher, subject1, subject2, subject3;
        teacher = items.at(i).toElement().attribute("name");
        subject1 = items.at(i).toElement().attribute("subject1");
        subject2 = items.at(i).toElement().attribute("subject2");
        subject3 = items.at(i).toElement().attribute("subject3");

        // Adding teachers and subjects to list
        if(!listTeachers.contains(teacher))
            listTeachers << teacher;
        if(!listSubjects.contains(subject1))
            listSubjects << subject1;
        if(!listSubjects.contains(subject2))
            listSubjects << subject2;
        if(!listSubjects.contains(subject3))
            listSubjects << subject3;

        // For making pairs
        QStringList tempSubList;
        tempSubList << subject1 << subject2 << subject3;

        TS.append(
                    qMakePair(
                        items.at(i).toElement().attribute("name"),
                        tempSubList));
    }
}

void MainWindow::on_buttonReGen_clicked()
{
    LoadTeacherInfo();

    // Get row and column count
    nRow = ui->spinDPW->value();
    nColumn = ui->spinCPD->value();

    // Create table
    ui->table->setRowCount(nRow);
    ui->table->setColumnCount(nColumn);

    //Set Vertical Labels by weekday names
    QList<QString>Days, vDays, hTimes;
    Days << "Saturday" << "Sunday" << "Monday" << "Tuesday" << "Wednesday" << "Thursday" << "Friday";
    for(int i=ui->comboDays->currentIndex(); i<=ui->spinDPW->value();i++)
        vDays << Days.at(i);
    ui->table->setVerticalHeaderLabels(vDays);

    // Set Horizontal Labels by time diffs
    for(int i=0;i<=ui->spinCPD->value();i++)
        hTimes << ui->timeStart->time().addSecs(i* (ui->timeDuration->time().hour() * 3600 + ui->timeDuration->time().minute()*60)).toString("hh:mm a");
    ui->table->setHorizontalHeaderLabels(hTimes);

    // Start the loop
    for(int dayCount=0;dayCount<nRow;dayCount++)
    {
        int cellCount=0;
        std::random_shuffle(cArray.begin() ,cArray.end());
        QList<QString>Taken;
        foreach (int classCount, cArray)// Finding teacher for nClass per day
        {
            // Find A Teacher From The List Of Subjects
            //for(int i=0;i<listTeachers.size();i++) // Finding teacher for that subject
            foreach (int i, tArray)
                if(!Taken.contains(listTeachers.at(i)) &&
                        TS.at(i).second.contains(listSubjects.at(classCount)))
                {
                    QTableWidgetItem *newItem = new QTableWidgetItem(QString("%1%2%3").arg(listSubjects.at(classCount)).arg(" : ").arg(TS.at(i).first));
                    ui->table->setItem(dayCount, cellCount, newItem); // Add to table cell
                    Taken.append(listTeachers.at(i));
                    break;
                } // Found a teacher
            cellCount++;
        } // End of Class loop
    } // End of Days loop
} // End of MainWindow

void MainWindow::on_comboDays_currentIndexChanged(int index)
{
    // Sat = 0, Sun = 1
    if(0 == index)
        ui->spinDPW->setMaximum(6);
    else
        ui->spinDPW->setMaximum(5);
}

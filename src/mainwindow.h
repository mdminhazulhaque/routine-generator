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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtXml>
#include <QMessageBox>
#include <algorithm>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void LoadTeacherInfo();
    
private slots:
    void on_buttonReGen_clicked();
    void on_comboDays_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    QList< QPair<QString,QStringList> >TS;
    QList<QString>listSubjects, listTeachers;
    int nRow, nColumn;
    std::vector<int>cArray;
    std::vector<int>tArray;
    QDomDocument document;
};

#endif // MAINWINDOW_H

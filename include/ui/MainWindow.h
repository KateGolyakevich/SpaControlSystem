#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTabWidget>
#include <QPushButton>
#include "core/SpaSystem.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    SpaSystem spa_system;

    // Элементы интерфейса
    QTabWidget* tab_widget;
    QTableWidget* table_appointments;
    QTableWidget* table_clients;
    QTableWidget* table_employees;
    QTableWidget* table_rooms;
    QTableWidget* table_services;
    QTableWidget* table_inventory;

    void setup_ui();
    void refresh_all_tables();
    void refresh_appointments_table();
    void refresh_clients_table();
    void refresh_employees_table();
    void refresh_rooms_table();
    void refresh_services_table();
    void refresh_inventory_table();

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void on_btn_add_appointment_clicked();
    void on_btn_complete_appointment_clicked();
    void on_btn_create_package_clicked();
    void on_btn_export_receipt_clicked();
    void on_btn_refresh_clicked();
};

#endif //MAINWINDOW_H
#include "ui/MainWindow.h"
#include "ui/AddAppointmentDialog.h"
#include "ui/SpaPackageBuilderDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), spa_system("data") {
    setup_ui();
    refresh_all_tables();
}

void MainWindow::setup_ui() {
    setWindowTitle("AuraSPA — Система управления спа-салоном");
    resize(1150, 700);

    auto* central_widget = new QWidget(this);
    auto* main_layout = new QVBoxLayout(central_widget);

    // Панель управления (кнопки)
    auto* toolbar_layout = new QHBoxLayout();
    auto* btn_add_app = new QPushButton("➕ Записать клиента", this);
    auto* btn_complete_app = new QPushButton("✔️ Завершить сеанс", this);
    auto* btn_create_pkg = new QPushButton("🌿 Конструктор СПА-пакетов", this);
    auto* btn_export_receipt = new QPushButton("🧾 Экспорт чека", this);
    auto* btn_refresh = new QPushButton("🔄 Обновить", this);

    btn_add_app->setStyleSheet("background-color: #27ae60; color: white; font-weight: bold; padding: 6px;");
    btn_complete_app->setStyleSheet("background-color: #2980b9; color: white; font-weight: bold; padding: 6px;");
    btn_create_pkg->setStyleSheet("background-color: #8e44ad; color: white; font-weight: bold; padding: 6px;");

    toolbar_layout->addWidget(btn_add_app);
    toolbar_layout->addWidget(btn_complete_app);
    toolbar_layout->addWidget(btn_create_pkg);
    toolbar_layout->addWidget(btn_export_receipt);
    toolbar_layout->addStretch();
    toolbar_layout->addWidget(btn_refresh);
    main_layout->addLayout(toolbar_layout);

    // Вкладки
    tab_widget = new QTabWidget(this);
    table_appointments = new QTableWidget(this);
    table_clients = new QTableWidget(this);
    table_employees = new QTableWidget(this);
    table_rooms = new QTableWidget(this);
    table_services = new QTableWidget(this);
    table_inventory = new QTableWidget(this);

    tab_widget->addTab(table_appointments, "📅 Журнал записей");
    tab_widget->addTab(table_clients, "👥 Клиенты");
    tab_widget->addTab(table_employees, "💆 Мастера и персонал");
    tab_widget->addTab(table_rooms, "🚪 СПА-зоны и кабинеты");
    tab_widget->addTab(table_services, "📋 Каталог услуг и пакетов");
    tab_widget->addTab(table_inventory, "📦 Склад расходников");

    main_layout->addWidget(tab_widget);
    setCentralWidget(central_widget);

    connect(btn_add_app, &QPushButton::clicked, this, &MainWindow::on_btn_add_appointment_clicked);
    connect(btn_complete_app, &QPushButton::clicked, this, &MainWindow::on_btn_complete_appointment_clicked);
    connect(btn_create_pkg, &QPushButton::clicked, this, &MainWindow::on_btn_create_package_clicked);
    connect(btn_export_receipt, &QPushButton::clicked, this, &MainWindow::on_btn_export_receipt_clicked);
    connect(btn_refresh, &QPushButton::clicked, this, &MainWindow::on_btn_refresh_clicked);
}

void MainWindow::refresh_all_tables() {
    refresh_appointments_table();
    refresh_clients_table();
    refresh_employees_table();
    refresh_rooms_table();
    refresh_services_table();
    refresh_inventory_table();
}

void MainWindow::refresh_appointments_table() {
    table_appointments->clear();
    table_appointments->setColumnCount(7);
    table_appointments->setHorizontalHeaderLabels({"ID", "Дата и время", "Клиент", "Специалист", "Кабинет / Зона", "Услуга / Пакет", "Статус"});
    table_appointments->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    const auto& apps = spa_system.get_appointment_repo().get_all();
    table_appointments->setRowCount(static_cast<int>(apps.size()));

    for (int i = 0; i < static_cast<int>(apps.size()); ++i) {
        const auto& app = apps[i];
        auto c = spa_system.get_client_repo().find_by_id(app.get_client_id());
        auto e = spa_system.get_employee_repo().find_by_id(app.get_employee_id());
        auto r = spa_system.get_room_repo().find_by_id(app.get_room_id());
        auto s = spa_system.find_service_by_id(app.get_service_id());

        table_appointments->setItem(i, 0, new QTableWidgetItem(QString::number(app.get_id())));
        table_appointments->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(app.get_date_time())));
        table_appointments->setItem(i, 2, new QTableWidgetItem(c ? QString::fromStdString(c->get_name()) : "Не найден"));
        table_appointments->setItem(i, 3, new QTableWidgetItem(e ? QString::fromStdString(e->get_name()) : "Не найден"));
        table_appointments->setItem(i, 4, new QTableWidgetItem(r ? QString::fromStdString(r->get_name()) : "Не найден"));
        table_appointments->setItem(i, 5, new QTableWidgetItem(s ? QString::fromStdString(s->get_title()) : "Не найден"));

        auto* status_item = new QTableWidgetItem(app.get_is_completed() ? "Завершен" : "Ожидается");
        if (app.get_is_completed()) {
            status_item->setForeground(QBrush(QColor("#7f8c8d")));
        } else {
            status_item->setForeground(QBrush(QColor("#27ae60")));
            status_item->setFont(QFont("Arial", 10, QFont::Bold));
        }
        table_appointments->setItem(i, 6, status_item);
    }
}

void MainWindow::refresh_clients_table() {
    table_clients->clear();
    table_clients->setColumnCount(5);
    table_clients->setHorizontalHeaderLabels({"ID", "ФИО Клиента", "Телефон", "Скидка лояльности", "Потрачено всего"});
    table_clients->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    const auto& clients = spa_system.get_client_repo().get_all();
    table_clients->setRowCount(static_cast<int>(clients.size()));

    for (int i = 0; i < static_cast<int>(clients.size()); ++i) {
        const auto& c = clients[i];
        table_clients->setItem(i, 0, new QTableWidgetItem(QString::number(c.get_id())));
        table_clients->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(c.get_name())));
        table_clients->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(c.get_phone_number())));
        table_clients->setItem(i, 3, new QTableWidgetItem(QString::number(c.get_discount_rate() * 100, 'f', 0) + "%"));
        table_clients->setItem(i, 4, new QTableWidgetItem(QString::number(c.get_total_spent(), 'f', 2) + " руб."));
    }
}

void MainWindow::refresh_employees_table() {
    table_employees->clear();
    table_employees->setColumnCount(5);
    table_employees->setHorizontalHeaderLabels({"ID", "ФИО Мастера", "Телефон", "Специализация", "Ставка за визит"});
    table_employees->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    const auto& emps = spa_system.get_employee_repo().get_all();
    table_employees->setRowCount(static_cast<int>(emps.size()));

    for (int i = 0; i < static_cast<int>(emps.size()); ++i) {
        const auto& e = emps[i];
        table_employees->setItem(i, 0, new QTableWidgetItem(QString::number(e.get_id())));
        table_employees->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(e.get_name())));
        table_employees->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(e.get_phone_number())));
        table_employees->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(e.get_specialty())));
        table_employees->setItem(i, 4, new QTableWidgetItem(QString::number(e.get_salary_rate(), 'f', 2) + " руб."));
    }
}

void MainWindow::refresh_rooms_table() {
    table_rooms->clear();
    table_rooms->setColumnCount(3);
    table_rooms->setHorizontalHeaderLabels({"ID", "Наименование СПА-зоны", "Категория / Тип"});
    table_rooms->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    const auto& rooms = spa_system.get_room_repo().get_all();
    table_rooms->setRowCount(static_cast<int>(rooms.size()));

    for (int i = 0; i < static_cast<int>(rooms.size()); ++i) {
        const auto& r = rooms[i];
        table_rooms->setItem(i, 0, new QTableWidgetItem(QString::number(r.get_id())));
        table_rooms->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(r.get_name())));
        table_rooms->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(r.get_type())));
    }
}

void MainWindow::refresh_services_table() {
    table_services->clear();
    table_services->setColumnCount(4);
    table_services->setHorizontalHeaderLabels({"ID", "Наименование услуги / СПА-пакета", "Длительность", "Стоимость"});
    table_services->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    const auto& services = spa_system.get_services();
    table_services->setRowCount(static_cast<int>(services.size()));

    for (int i = 0; i < static_cast<int>(services.size()); ++i) {
        const auto& s = services[i];
        table_services->setItem(i, 0, new QTableWidgetItem(QString::number(s->get_id())));
        table_services->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(s->get_title())));
        table_services->setItem(i, 2, new QTableWidgetItem(QString::number(s->get_duration()) + " мин."));
        table_services->setItem(i, 3, new QTableWidgetItem(QString::number(s->calculate_cost(), 'f', 2) + " руб."));
    }
}

void MainWindow::refresh_inventory_table() {
    table_inventory->clear();
    table_inventory->setColumnCount(4);
    table_inventory->setHorizontalHeaderLabels({"ID", "Наименование материала", "Остаток на складе", "Ед. изм."});
    table_inventory->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    const auto& inv = spa_system.get_inventory_repo().get_all();
    table_inventory->setRowCount(static_cast<int>(inv.size()));

    for (int i = 0; i < static_cast<int>(inv.size()); ++i) {
        const auto& item = inv[i];
        auto* name_item = new QTableWidgetItem(QString::fromStdString(item.get_name()));
        auto* qty_item = new QTableWidgetItem(QString::number(item.get_quantity(), 'f', 1));

        if (item.get_quantity() < 20.0) {
            name_item->setBackground(QBrush(QColor("#fadbd8")));
            qty_item->setBackground(QBrush(QColor("#fadbd8")));
            qty_item->setForeground(QBrush(QColor("#c0392b")));
            qty_item->setFont(QFont("Arial", 10, QFont::Bold));
        }

        table_inventory->setItem(i, 0, new QTableWidgetItem(QString::number(item.get_id())));
        table_inventory->setItem(i, 1, name_item);
        table_inventory->setItem(i, 2, qty_item);
        table_inventory->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(item.get_unit())));
    }
}

void MainWindow::on_btn_add_appointment_clicked() {
    AddAppointmentDialog dialog(&spa_system, this);
    if (dialog.exec() == QDialog::Accepted) {
        refresh_appointments_table();
    }
}

void MainWindow::on_btn_complete_appointment_clicked() {
    int current_row = table_appointments->currentRow();
    if (current_row < 0) {
        QMessageBox::warning(this, "Внимание", "Выберите запись в таблице для завершения!");
        return;
    }

    int app_id = table_appointments->item(current_row, 0)->text().toInt();

    try {
        spa_system.complete_appointment(app_id);
        refresh_all_tables();
        QMessageBox::information(this, "Сеанс завершен",
            QString("Сеанс #%1 успешно завершен!\nРасходники списаны со склада, клиенту начислены бонусы, чек сформирован.")
            .arg(app_id));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}

void MainWindow::on_btn_create_package_clicked() {
    SpaPackageBuilderDialog dialog(&spa_system, this);
    if (dialog.exec() == QDialog::Accepted) {
        refresh_services_table();
    }
}

void MainWindow::on_btn_export_receipt_clicked() {
    int current_row = table_appointments->currentRow();
    if (current_row < 0) {
        QMessageBox::warning(this, "Внимание", "Выберите запись для просмотра чека!");
        return;
    }
    int app_id = table_appointments->item(current_row, 0)->text().toInt();
    QString receipt_file = QString("data/receipt_app_%1.txt").arg(app_id);
    QMessageBox::information(this, "Экспорт чека", "Чек сохранен по пути: " + receipt_file);
}

void MainWindow::on_btn_refresh_clicked() {
    refresh_all_tables();
    QMessageBox::information(this, "Обновление", "Все таблицы успешно синхронизированы!");
}
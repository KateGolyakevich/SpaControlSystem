#include "ui/SpaPackageBuilderDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QPushButton> // <--- Добавили этот заголовок!

SpaPackageBuilderDialog::SpaPackageBuilderDialog(SpaSystem* system, QWidget* parent)
    : QDialog(parent), spa_system(system) {
    setup_ui();
    populate_available_services();
    update_summary();
}

void SpaPackageBuilderDialog::setup_ui() {
    setWindowTitle("Конструктор комплексных СПА-программ (Киллер-фича)");
    setFixedSize(650, 480);

    auto* main_layout = new QVBoxLayout(this);
    auto* form_layout = new QFormLayout();

    edit_title = new QLineEdit(this);
    edit_title->setPlaceholderText("Например: СПА-день \"Цитрусовый детокс\"");

    spin_discount = new QDoubleSpinBox(this);
    spin_discount->setRange(0.0, 50.0);
    spin_discount->setValue(15.0);
    spin_discount->setSuffix(" %");

    form_layout->addRow("Название СПА-пакета:", edit_title);
    form_layout->addRow("Комплексная скидка:", spin_discount);
    main_layout->addLayout(form_layout);

    auto* lists_layout = new QHBoxLayout();
    list_available = new QListWidget(this);
    list_selected = new QListWidget(this);

    auto* mid_btn_layout = new QVBoxLayout();
    auto* btn_add = new QPushButton(">>", this);
    auto* btn_remove = new QPushButton("<<", this);
    mid_btn_layout->addStretch();
    mid_btn_layout->addWidget(btn_add);
    mid_btn_layout->addWidget(btn_remove);
    mid_btn_layout->addStretch();

    lists_layout->addWidget(list_available);
    lists_layout->addLayout(mid_btn_layout);
    lists_layout->addWidget(list_selected);
    main_layout->addLayout(lists_layout);

    label_summary = new QLabel(this);
    label_summary->setStyleSheet("font-weight: bold; color: #2c3e50; font-size: 13px;");
    main_layout->addWidget(label_summary);

    auto* bottom_layout = new QHBoxLayout();
    auto* btn_save = new QPushButton("Сохранить СПА-пакет", this);
    auto* btn_close = new QPushButton("Закрыть", this);
    bottom_layout->addStretch();
    bottom_layout->addWidget(btn_close);
    bottom_layout->addWidget(btn_save);
    main_layout->addLayout(bottom_layout);

    connect(btn_add, &QPushButton::clicked, this, &SpaPackageBuilderDialog::on_btn_add_service_clicked);
    connect(btn_remove, &QPushButton::clicked, this, &SpaPackageBuilderDialog::on_btn_remove_service_clicked);
    connect(spin_discount, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &SpaPackageBuilderDialog::update_summary);
    connect(btn_save, &QPushButton::clicked, this, &SpaPackageBuilderDialog::on_btn_save_package_clicked);
    connect(btn_close, &QPushButton::clicked, this, &QDialog::reject);
}

void SpaPackageBuilderDialog::populate_available_services() {
    list_available->clear();
    for (const auto& s : spa_system->get_services()) {
        auto item = new QListWidgetItem(QString::fromStdString(s->get_title() + " (" +
                    std::to_string(s->get_duration()) + " мин, " +
                    std::to_string(static_cast<int>(s->calculate_cost())) + " руб.)"));
        item->setData(Qt::UserRole, s->get_id());
        list_available->addItem(item);
    }
}

void SpaPackageBuilderDialog::on_btn_add_service_clicked() {
    auto current_item = list_available->currentItem();
    if (!current_item) return;

    int service_id = current_item->data(Qt::UserRole).toInt();
    selected_service_ids.push_back(service_id);

    auto new_item = new QListWidgetItem(current_item->text());
    new_item->setData(Qt::UserRole, service_id);
    list_selected->addItem(new_item);

    update_summary();
}

void SpaPackageBuilderDialog::on_btn_remove_service_clicked() {
    int row = list_selected->currentRow();
    if (row < 0) return;

    selected_service_ids.erase(selected_service_ids.begin() + row);
    delete list_selected->takeItem(row);

    update_summary();
}

void SpaPackageBuilderDialog::update_summary() {
    double total_base = 0.0;
    int total_minutes = 0;

    for (int id : selected_service_ids) {
        auto s = spa_system->find_service_by_id(id);
        if (s) {
            total_base += s->calculate_cost();
            total_minutes += s->get_duration();
        }
    }

    double discount_rate = spin_discount->value() / 100.0;
    double final_cost = total_base * (1.0 - discount_rate);

    label_summary->setText(QString("Итого: Длительность: %1 мин. | Базовая: %2 руб. | С учетом скидки: %3 руб.")
        .arg(total_minutes)
        .arg(total_base, 0, 'f', 2)
        .arg(final_cost, 0, 'f', 2));
}

void SpaPackageBuilderDialog::on_btn_save_package_clicked() {
    QString title = edit_title->text().trimmed();
    if (title.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Введите наименование СПА-пакета!");
        return;
    }
    if (selected_service_ids.empty()) {
        QMessageBox::warning(this, "Внимание", "Добавьте хотя бы одну процедуру в пакет!");
        return;
    }

    double discount = spin_discount->value() / 100.0;
    spa_system->create_spa_package(title.toStdString(), discount, selected_service_ids);

    QMessageBox::information(this, "Успех", "СПА-пакет успешно сохранен в каталоге услуг!");
    accept();
}
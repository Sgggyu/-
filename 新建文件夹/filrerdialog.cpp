#include "filterdialog.h"

FilterDialog::FilterDialog(const QString &columnName, const QStringList &values, QWidget *parent)
    : QDialog(parent), allValues(values)
{
    setWindowTitle(QString("筛选: %1").arg(columnName));

    // 创建 UI 组件
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText("搜索...");

    selectAllCheckBox = new QCheckBox("全选/取消全选", this);
    listWidget = new QListWidget(this);
    listWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    applyButton = new QPushButton("应用", this);

    // 布局设置
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(searchLineEdit);
    mainLayout->addWidget(selectAllCheckBox);
    mainLayout->addWidget(listWidget);
    mainLayout->addWidget(applyButton);
    setLayout(mainLayout);

    // 填充完整选项列表
    updateListWidget(allValues);

    // 连接信号与槽
    connect(searchLineEdit, &QLineEdit::textChanged, this, &FilterDialog::onSearchTextChanged);
    connect(selectAllCheckBox, &QCheckBox::toggled, this, &FilterDialog::onSelectAllToggled);
    connect(applyButton, &QPushButton::clicked, this, &FilterDialog::onApplyButtonClicked);
}

FilterDialog::~FilterDialog() {}


void FilterDialog::onSelectAllToggled(bool checked)
{
    // 遍历列表中的所有项，设置选中状态
    for (int i = 0; i < listWidget->count(); ++i) {
        QListWidgetItem *item = listWidget->item(i);
        item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
    }
}

void FilterDialog::onSearchTextChanged(const QString &text)
{
    // 筛选匹配项
    QStringList filteredValues;
    for (const QString &value : allValues) {
        if (value.contains(text, Qt::CaseInsensitive)) {
            filteredValues.append(value);
        }
    }

    // 更新列表显示
    updateListWidget(filteredValues);
}

void FilterDialog::onApplyButtonClicked()
{
    QStringList selectedValues;
    for (int i = 0; i < listWidget->count(); ++i) {
        QListWidgetItem *item = listWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            selectedValues.append(item->text());
        }
    }

    emit filterApplied(selectedValues);
    accept();
}


void FilterDialog::updateListWidget(const QStringList &values)
{
    listWidget->clear();

    for (const QString &value : values) {
        QListWidgetItem *item = new QListWidgetItem(value, listWidget);
        item->setCheckState(Qt::Unchecked);  // 初始未选中
        listWidget->addItem(item);
    }
}

#include "serverconfiglist.h"
#include "operationconfirmdlg.h"
#include "serverconfigitem.h"
#include "serverconfigmanager.h"
#include "serverconfigoptdlg.h"
#include "serverconfigdelegate.h"
#include "serverconfigitemmodel.h"
#include <DLog>
ServerConfigList::ServerConfigList(QWidget *parent) : DListView(parent)
{
    setBackgroundRole(QPalette::NoRole);
    setAutoFillBackground(false);
    setUpdatesEnabled(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    initData();
}

void ServerConfigList::initData()
{
    m_serCfgListModel = new QStandardItemModel;
    m_serCfgProxyModel = new ServerConfigItemModel(this);
    m_serCfgProxyModel->setSourceModel(m_serCfgListModel);
    m_serCfgProxyModel->setFilterRole(Qt::UserRole);
    m_serCfgProxyModel->setDynamicSortFilter(true);

    m_serCfgDelegate = new ServerConfigDelegate(this);
    this->setModel(m_serCfgProxyModel);
    this->setItemDelegate(m_serCfgDelegate);
}

void ServerConfigList::refreshAllDatas()
{
    state = 1;
    clearData();
    QMap<QString, QList<ServerConfig *>> &configMap = ServerConfigManager::instance()->getServerConfigs();
    QMap<QString, QList<ServerConfig *>>::const_iterator iter = configMap.constBegin();
    while (iter != configMap.constEnd()) {
        if (iter.key().isEmpty()) {
            iter++;
            continue;
        }
        int count = configMap[iter.key()].count();
        ServerConfigItemData itemData;
        itemData.m_group = iter.key();
        itemData.m_number = QString("%1 server").arg(count);
        m_serCfgItemDataList.append(itemData);
//        m_serCfgProxyModel->addNewServerData(itemData);
        iter++;
    }
    refreshDataByGroup("", false);
}

ServerConfigItemData changePointerToObject(ServerConfig *cfg)
{
    ServerConfigItemData itemData;
    itemData.m_userName = cfg->m_userName;
    itemData.m_address = cfg->m_address;
    itemData.m_port = cfg->m_port;
    itemData.m_serverName = cfg->m_serverName;
    itemData.m_password = cfg->m_password;
    itemData.m_group = cfg->m_group;
    itemData.m_command = cfg->m_command;
    itemData.m_path = cfg->m_path;
    itemData.m_encoding = cfg->m_encoding;
    itemData.m_backspaceKey = cfg->m_backspaceKey;
    itemData.m_deleteKey = cfg->m_deleteKey;
    itemData.m_privateKey = cfg->m_privateKey;
    return itemData;
}

void ServerConfigList::refreshDataByGroup(const QString &strGroupName, bool isFromGroupPanel)
{
    m_GroupName = strGroupName;
    if (isFromGroupPanel) {
        state = 2;
    }
    QMap<QString, QList<ServerConfig *>> &configMap = ServerConfigManager::instance()->getServerConfigs();
    QList<ServerConfig *> &configList = configMap[strGroupName];
    for (auto cfg : configList) {
        ServerConfigItemData itemData = changePointerToObject(cfg);
        if (isFromGroupPanel) {
            itemData.m_IsInGrouppanel = true;
        }
        m_serCfgItemDataList.append(itemData);
    }
    m_serCfgProxyModel->initServerListData(m_serCfgItemDataList);
}

//根据分组信息和搜索信息联合查询
void ServerConfigList::refreshDataByGroupAndFilter(const QString &strGroupName, const QString &strFilter)
{
    state = 4;
    m_GroupName = strGroupName;
    m_Filter = strFilter;
    QMap<QString, QList<ServerConfig *>> &configMap = ServerConfigManager::instance()->getServerConfigs();
    QList<ServerConfig *> &configList = configMap[strGroupName];
    for (auto cfg : configList) {
        if (cfg->m_serverName.contains(strFilter, Qt::CaseSensitivity::CaseInsensitive)
                || cfg->m_userName.contains(strFilter, Qt::CaseSensitivity::CaseInsensitive)) {
            ServerConfigItemData itemData = changePointerToObject(cfg);
            m_serCfgItemDataList.append(itemData);
        }
    }
    m_serCfgProxyModel->initServerListData(m_serCfgItemDataList);
}

void ServerConfigList::refreshDataByFilter(const QString &strFilter)
{
    state = 3;
    m_Filter = strFilter;
    QMap<QString, QList<ServerConfig *>> &configMap = ServerConfigManager::instance()->getServerConfigs();
    QMap<QString, QList<ServerConfig *>>::const_iterator iter = configMap.constBegin();
    while (iter != configMap.constEnd()) {
        QList<ServerConfig *> configList = iter.value();
        for (auto cfg : configList) {
            if (cfg->m_serverName.contains(strFilter, Qt::CaseSensitivity::CaseInsensitive)
                    || cfg->m_userName.contains(strFilter, Qt::CaseSensitivity::CaseInsensitive)) {
                ServerConfigItemData itemData = changePointerToObject(cfg);
                m_serCfgItemDataList.append(itemData);
            }
        }
        iter++;
    }
    m_serCfgProxyModel->initServerListData(m_serCfgItemDataList);
}

void ServerConfigList::refreshOneRowServerConfigInfo(ServerConfig *curConfig)
{
//    for (int i = 0; i < count(); i++) {
//        ServerConfigItem *curItem = qobject_cast<ServerConfigItem *>(itemWidget(item(i)));
//        if (curItem->getCurServerConfig() == curConfig) {
//            // curItem->refresh(action);
//        }
//    }
}

void ServerConfigList::addOneRowServerConfigGroupData(const QString &group)
{
//    ServerConfigItem *configItem = new ServerConfigItem(nullptr, true, group, this);
//    QListWidgetItem *item = new QListWidgetItem();
//    item->setSizeHint(QSize(260, 70));
//    addItem(item);
//    this->setItemWidget(item, configItem);
//    item->setData(Qt::UserRole, 0);
//    connect(configItem, &ServerConfigItem::modifyServerConfig, this, &ServerConfigList::handleModifyServerConfig);
}

void ServerConfigList::addOneRowServerConfigData(ServerConfig *curConfig)
{
//    ServerConfigItem *configItem = new ServerConfigItem(curConfig, false, "", this);
//    QListWidgetItem *item = new QListWidgetItem();
//    item->setSizeHint(QSize(260, 70));
//    addItem(item);
//    this->setItemWidget(item, configItem);
//    connect(configItem, &ServerConfigItem::modifyServerConfig, this, &ServerConfigList::handleModifyServerConfig);
}

int ServerConfigList::getItemRow(ServerConfigItemData itemData)
{
//    for (int i = 0; i < count(); i++) {
//        CustomCommandItemData data = m_cmdItemDataList.at(i);
//        if (itemData.m_cmdName == data.m_cmdName
//                && itemData.m_cmdText == data.m_cmdText
//                && itemData.m_cmdShortcut == data.m_cmdShortcut) {
//            return i;
//        }
//    }
//    return -1;
}

void ServerConfigList::handleModifyServerConfig(ServerConfig *curItemServer, QModelIndex modelIndex)
{
    ServerConfigOptDlg dlg(ServerConfigOptDlg::SCT_MODIFY, curItemServer, this);
    if (dlg.exec() == QDialog::Accepted) {
        if (dlg.isDelServer()) {
            OperationConfirmDlg optDlg;
            optDlg.setOperatTypeName(tr("delete opt"));
            optDlg.setTipInfo(tr("Do you sure to delete the %1").arg(curItemServer->m_serverName));
            optDlg.exec();
            if (optDlg.getConfirmResult() == QDialog::Accepted) {
//                takeItem(getItemRow(item));
                ServerConfigManager::instance()->delServerConfig(curItemServer);
                refreshPanelData(modelIndex);
                emit listItemCountChange();
            }
        } else {
            //刷新所有数据，待完善
            refreshPanelData(modelIndex);
        }
    }
}

void ServerConfigList::refreshPanelData(QModelIndex modelIndex)
{
    Q_UNUSED(modelIndex)
    if (m_serCfgListModel && m_serCfgListModel->rowCount() == 0) {
        return;
    }
    switch (state) {
    case 1:
        refreshAllDatas();
        break;
    case 2:
        clearData();
        refreshDataByGroup(m_GroupName, true);
        break;
    case 3:
        clearData();
        refreshDataByFilter(m_Filter);
        break;
    case 4:
        clearData();
        refreshDataByGroupAndFilter(m_GroupName, m_Filter);
        break;
    }
}

QRect getModifyIconRectS(QRect visualRect)
{
    int modifyIconSize = 30;
    return QRect(visualRect.right() - modifyIconSize - 10, visualRect.top() + (visualRect.height() - modifyIconSize) / 2, modifyIconSize, modifyIconSize);
}

void ServerConfigList::mouseMoveEvent(QMouseEvent *event)
{
    DListView::mouseMoveEvent(event);
}

void ServerConfigList::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_bLeftMouse = true;
    } else {
        m_bLeftMouse = false;
    }

    DListView::mousePressEvent(event);

    if (m_serCfgListModel && m_serCfgListModel->rowCount() == 0) {
        return;
    }

    if (!m_serCfgListModel) {
        return;
    }

    QPoint clickPoint = event->pos();

    QModelIndex modelIndex = indexAt(clickPoint);
    QRect rect = visualRect(modelIndex);

    if (!modelIndex.isValid()) {
        return;
    }

    ServerConfigItemData itemData =
        qvariant_cast<ServerConfigItemData>(m_serCfgProxyModel->data(modelIndex));
    ServerConfig *curItemServer = new ServerConfig();
    curItemServer->m_userName = itemData.m_userName;
    curItemServer->m_address = itemData.m_address;
    curItemServer->m_port = itemData.m_port;
    curItemServer->m_serverName = itemData.m_serverName;
    curItemServer->m_password = itemData.m_password;
    curItemServer->m_group = itemData.m_group;
    curItemServer->m_command = itemData.m_command;
    curItemServer->m_path = itemData.m_path;
    curItemServer->m_encoding = itemData.m_encoding;
    curItemServer->m_backspaceKey = itemData.m_backspaceKey;
    curItemServer->m_deleteKey = itemData.m_deleteKey;
    curItemServer->m_privateKey = itemData.m_privateKey;

    if (getModifyIconRectS(rect).contains(clickPoint)) {
        handleModifyServerConfig(curItemServer, modelIndex);
//        m_serCfgProxyModel->setData(modelIndex, QVariant::fromValue(itemData), Qt::DisplayRole);
    } else {
        emit itemClicked(curItemServer);
    }
}

void ServerConfigList::mouseReleaseEvent(QMouseEvent *event)
{
    DListView::mouseReleaseEvent(event);
}

void ServerConfigList::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command)
{
    DListView::setSelection(rect, command);
}

void ServerConfigList::clearData()
{
    m_serCfgListModel->clear();
    m_serCfgItemDataList.clear();
}


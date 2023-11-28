#ifndef LEFTMENUMODEL_H
#define LEFTMENUMODEL_H

#include <QObject>
#include <QAbstractListModel>

enum MENU_LEVEL{
    MENU_LEVEL_FIRST = 0,
    MENU_LEVEL_SECOND,
    MENU_LEVEL_THIRD,
    MENU_LEVEL_NONE
};

class MenuItem{
    Q_GADGET
public:
    MenuItem(const QString& name,const int& level,int index,const int& unfold = 0){
        m_name = name;
        m_level = level;
        m_unfold = unfold;
        m_index = index;
    }
    MenuItem(){

    }
    Q_PROPERTY(QString name READ name WRITE setName)

    QString name(){
        return m_name;
    }
    int level(){
        return m_level;
    }
    int unfold(){
        return m_unfold;
    }
    int index(){
        return m_index;
    }
    void setName(QString name){
        m_name = name;
    }
    void setLevel(int level){
        m_level = level;
    }
    void setUnfold(int unfold){
        m_unfold = unfold;
    }
    void setIndex(int index){
        m_index = index;
    }
private:
    //ui显示
    QString m_name;
    //菜单级别，从0开始
    int m_level;
    //是否展开
    int m_unfold;
    //菜单index
    int m_index;
};
Q_DECLARE_METATYPE(MenuItem)

class LeftMenuModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ItemRoles{
        TypeName = Qt::UserRole + 1,
        TypeLevel,
        TypeUnfold,
        TypeIndex
    };
    LeftMenuModel(QObject *parent = nullptr);
    void init();

    //item操作
    void appendItem(const MenuItem& item);
    void insertItem(const MenuItem& item,int index);
    void removeItem(int index);
    void changeItem(const MenuItem& item,int index);

    int rowCount(const QModelIndex & parent = QModelIndex())const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    int length();
    void setUnfold(int index,int value);
protected:
    QHash<int, QByteArray> roleNames() const;
public:
    //对外数据
    QList<MenuItem>m_items;
    //菜单初始化
    std::vector<std::pair<QString,QList<QString>>>m_menu;
public slots:

signals:

};

#endif // LEFTMENUMODEL_H

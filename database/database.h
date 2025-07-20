#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariantList>

class Database : public QObject {
  Q_OBJECT
public:
  explicit Database(QObject *parent = nullptr);
  Q_INVOKABLE void test();
  Q_INVOKABLE QVariantList getTasks();
  Q_INVOKABLE QVariantMap addTask(const QString &title);
  Q_INVOKABLE bool toggleTask(const int id);
  Q_INVOKABLE bool delTask(const int id);

private:
  QSqlDatabase db;
  bool initDatabase();
  /**
   * @brief Запрос для манипуляции с данными
   * @details Детали
   */
  QSqlQuery query;

signals:
};

#endif // DATABASE_H

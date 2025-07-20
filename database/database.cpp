#include "database.h"
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QSqlError>

Database::Database(QObject *parent) : QObject{parent} { initDatabase(); }

void Database::test() { qDebug() << "test"; }

QVariantList Database::getTasks() {
  QVariantList tasks;

  query.clear();
  if (!query.exec("SELECT * FROM tasks ORDER BY id DESC")) {
    qDebug() << "Database::getTasks() error: " << query.lastError().text();
    return tasks;
  };

  while (query.next()) {
    QVariantMap task;

    task["id"] = query.value("id");
    task["title"] = query.value("title");
    task["createAt"] = query.value("create_at");
    task["isDone"] = query.value("isDone");

    tasks.append(task);
  }
  /* Для отладки
    for (int i = 1; i < 6; i++) {
      QVariantMap task;
      task["id"] = i;
      task["title"] = "title" + QString::number(i);
      bool isDone = i % 2 == 0;
      task["isDone"] = isDone;

      tasks.append(task);
    }
  */

  return tasks;
}

QVariantMap Database::addTask(const QString &title) {
  QSqlQuery query;
  query.prepare(R"(
        INSERT INTO tasks(title, create_at) VALUES(:Title, :CreateAt);
    )");
  query.bindValue(":Title", title);
  QDateTime now = QDateTime::currentDateTime();
  QString nowStr = now.toString(Qt::ISODate);
  query.bindValue(":CreateAt", nowStr);

  QVariantMap result;
  result["id"] = 0;
  result["createAt"] = nowStr;
  result["isDone"] = false;
  result["title"] = title;

  if (query.exec()) {
    if (query.exec("SELECT last_insert_rowid() as InsertedId;")) {
      if (query.next()) {
        int id = query.value(0).toInt();
        result["id"] = id;
      } else {
        qDebug() << query.lastError().text();
      }
    } else {
      qDebug() << query.lastError().text();
    }
  } else {
    qDebug() << query.lastError().text();
  }

  return result;
}

bool Database::toggleTask(const int id) {
  QSqlQuery query;

  query.prepare(R"(
        UPDATE tasks SET isDone = iif(isDone = 0, 1, 0) WHERE id = :Id
      )");
  query.bindValue(":Id", id);

  bool result = query.exec();

  if(!result)
      qDebug() << "Database::toggleTask error: " << query.lastError().text();

  return result;
}

bool Database::delTask(const int id) {
    query.clear();
    query.prepare("DELETE FROM tasks WHERE id = :id");
    query.bindValue(":id", id);
    bool result = query.exec();

    if(!result)
        qDebug() << "Database::delTask error: " << query.lastError().text();

    return result;
}

bool Database::initDatabase() {
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName("todo.db");

  qDebug() << "Current path:" << QDir::currentPath();

  if (!db.open()) {
    qWarning() << "Database error: " << db.lastError().text();
    return false;
  }

  QSqlQuery query;
  QString createTable = R"(
        CREATE TABLE IF NOT EXISTS tasks(
        id INTEGER	PRIMARY KEY AUTOINCREMENT,
        title TEXT NOT NULL,
        create_at TEXT NOT NULL,
        isDone INTEGER DEFAULT 0)
    )";

  //Повторная инициалицация для того, чтобы запрос видел подключение)
  this->query = QSqlQuery();

  return query.exec(createTable);
}

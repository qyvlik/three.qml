#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include <QtDebug>


//class Struct
//{
//    Q_GADGET
//public:

//    Struct():
//        age(0)
//    {}

//    Q_INVOKABLE int getAge() const;
//    Q_INVOKABLE Struct& setAge(int value);

//private:
//    int age;
//};

//Q_DECLARE_METATYPE(Struct)

//class SampleTest : public QObject {
//    Q_OBJECT
//public:
//    SampleTest(QObject* parent = 0):
//        QObject(parent)
//    {}

//    Q_INVOKABLE Struct createStruct() const {
//        return Struct();
//    }
//};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//    qmlRegisterType<SampleTest>("Test", 1, 0, "SampleTest");
//    qRegisterMetaType<Struct>("Struct");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

//int Struct::getAge() const
//{
//    return age;
//}

//Struct& Struct::setAge(int value)
//{
//    age = value;

//    qDebug() << this;
//    return *this;
//}



//#include "main.moc"



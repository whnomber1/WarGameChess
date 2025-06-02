#include "CJsonVariant.h"
#include <QJsonObject>
#include <QJsonArray>

CJsonVariant::CJsonVariant(QObject *parent)
    : QObject{parent}
{

}

CJsonVariant::CJsonVariant(QString str)
{
    m_document = QJsonDocument::fromJson(str.toUtf8());
}

void CJsonVariant::SetJsonString(QString jsonstr)
{
    m_document = QJsonDocument::fromJson(jsonstr.toUtf8());
}

bool CJsonVariant::isValid(QString str)
{
    bool retb = true;
    QStringList lst = str.split("/", QString::SkipEmptyParts);
    QJsonObject jsonObject = m_document.object();
    QJsonValue val = jsonObject;
    for(int i=0; i<lst.size();i++)
    {
        bool isNum, islast=(i==lst.size()-1);
        int num = lst.at(i).toInt(&isNum);
        if(isNum)
        {
            if(!val.isArray())
            {
                retb = false;
                break;
            }
            QJsonArray arr = val.toArray();
            if(num >= arr.size())
            {
                retb = false;
                break;
            }
            val = arr.at(num);
        }
        else
        {
            if(!val.isObject() || !val.toObject().contains(lst.at(i)))
            {
                retb = false;
                break;
            }
            val = val.toObject().value(lst.at(i));
        }
    }
    return retb;
}

QJsonValue CJsonVariant::getValue(QString keystr)
{
    QJsonValue retv;
    bool retb = true;
    QStringList lst = keystr.split("/", QString::SkipEmptyParts);
    QJsonObject jsonObject = m_document.object();
    QJsonValue val = jsonObject;
    for(int i=0; i<lst.size();i++)
    {
        bool isNum, islast=(i==lst.size()-1);
        int num = lst.at(i).toInt(&isNum);
        if(isNum)
        {
            if(!val.isArray())
            {
                retb = false;
                break;
            }
            QJsonArray arr = val.toArray();
            if(num >= arr.size())
            {
                retb = false;
                break;
            }
            val = arr.at(num);
        }
        else
        {
            if(!val.isObject() || !val.toObject().contains(lst.at(i)))
            {
                retb = false;
                break;
            }
            val = val.toObject().value(lst.at(i));
        }
    }
    if(retb)
    {
        retv = val;
    }
    return retv;
}

void CJsonVariant::setValue(QString keystr, QJsonValue vlu)
{
    bool retb = true;
    QStringList lst = keystr.split("/", QString::SkipEmptyParts);
    //    QJsonDocument doc;
    QJsonValue val;
    if(m_document.isObject())
    {
        val = m_document.object();
    }
    else if(m_document.isArray())
    {
        val = m_document.array();
    }
    QVector<QJsonValue> jvalstack;
    //    jvalstack.push_back(val);
    for(int i=0; i<lst.size();i++)
    {
        bool isNum, islast=(i==lst.size()-1);
        int num = lst.at(i).toInt(&isNum);
        if(!islast)
        {
            //            if(val.isArray())
            //            {
            //                doc.setArray(val.toArray());
            //            }
            //            else if(val.isObject())
            //            {
            //                doc.setObject(val.toObject());
            //            }
            //            qDebug() << lst.at(i) << "----" << doc.toJson(QJsonDocument::Compact);
            jvalstack.push_back(val);
        }
        if(isNum)
        {
            QJsonArray arr = val.toArray();
            if(islast)
            {
                if(num>=arr.size())
                {
                    arr.append(vlu);
                }
                else
                {
                    arr[num]=vlu;
                }
                val = arr;
            }
            else if(arr.size() > num)
            {
                val = arr.at(num);
            }
            else
            {
                val = QJsonObject();
            }
        }
        else
        {
            QJsonObject obj = val.toObject();
            if(islast)
            {
                obj.insert(lst.at(i), vlu);
                val = obj;
            }
            else if(obj.contains(lst.at(i)))
            {
                val = obj.value(lst.at(i));
            }
            else
            {
                val = QJsonObject();
            }
        }
    }
    //    if(val.isArray())
    //    {
    //        doc.setArray(val.toArray());
    //    }
    //    else if(val.isObject())
    //    {
    //        doc.setObject(val.toObject());
    //    }
    //    qDebug() << doc.toJson(QJsonDocument::Compact);
    QJsonValue valtmp = val;

    for(int i=lst.size()-2; i>=0; i--)
    {
        bool isNum, islast=(i==lst.size()-1);
        int num = lst.at(i).toInt(&isNum);
        if(isNum)
        {
            QJsonArray valarr = jvalstack.back().toArray();
            if(num>=valarr.size())
            {
                valarr.append(valtmp);
            }
            else
            {
                valarr[num]=valtmp;
            }
            //            doc.setArray(valarr);
            //            qDebug() << "----" << doc.toJson(QJsonDocument::Compact);
            valtmp = valarr;
            //            if(i)
            {
                jvalstack.pop_back();
            }
        }
        else
        {
            QJsonObject valobj = jvalstack.back().toObject();
            valobj.insert(lst.at(i), valtmp);
            //            doc.setObject(valobj);
            //            qDebug() << "----" << doc.toJson(QJsonDocument::Compact);
            valtmp = valobj;
            //            if(i)
            {
                jvalstack.pop_back();
            }
        }
    }
    if(valtmp.isArray())
    {
        m_document.setArray(valtmp.toArray());
    }
    else if(valtmp.isObject())
    {
        m_document.setObject(valtmp.toObject());
    }
}

QString CJsonVariant::GetJsonString()
{
    return m_document.toJson(QJsonDocument::Compact);
}

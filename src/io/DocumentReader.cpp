#include "io/DocumentReader.h"

DocumentReader::DocumentReader()
    : m_hasError(false)
{
}

Document *DocumentReader::read(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_hasError = true;
        m_errorString = "无法打开文件: " + file.errorString();
        return nullptr;
    }
    
    QTextStream stream(&file);
    Document *document = read(stream);
    file.close();
    
    return document;
}

Document *DocumentReader::read(QTextStream &stream)
{
    Document *document = new Document();
    
    try {
        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            Paragraph paragraph;
            paragraph.setText(line);
            document->addParagraph(paragraph);
        }
        
        m_hasError = false;
        m_errorString = "";
    }
    catch (const std::exception &e)
    {
        m_hasError = true;
        m_errorString = "读取文件时发生错误: " + QString::fromStdString(e.what());
        delete document;
        return nullptr;
    }
    
    return document;
}

bool DocumentReader::hasError() const
{
    return m_hasError;
}

QString DocumentReader::errorString() const
{
    return m_errorString;
}

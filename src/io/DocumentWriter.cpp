#include "io/DocumentWriter.h"

DocumentWriter::DocumentWriter()
    : m_hasError(false)
{
}

bool DocumentWriter::write(const Document *document, const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        m_hasError = true;
        m_errorString = "无法打开文件: " + file.errorString();
        return false;
    }
    
    QTextStream stream(&file);
    bool result = write(document, stream);
    file.close();
    
    return result;
}

bool DocumentWriter::write(const Document *document, QTextStream &stream)
{
    if (!document)
    {
        m_hasError = true;
        m_errorString = "文档为空";
        return false;
    }
    
    try {
        for (int i = 0; i < document->paragraphCount(); i++)
        {
            Paragraph paragraph = document->paragraph(i);
            stream << paragraph.text() << "\n";
        }
        
        m_hasError = false;
        m_errorString = "";
        return true;
    }
    catch (const std::exception &e)
    {
        m_hasError = true;
        m_errorString = "写入文件时发生错误: " + QString::fromStdString(e.what());
        return false;
    }
}

bool DocumentWriter::hasError() const
{
    return m_hasError;
}

QString DocumentWriter::errorString() const
{
    return m_errorString;
}

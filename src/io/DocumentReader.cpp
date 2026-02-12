// ============================================================================
// DocumentReader.cpp
// 文档读取器类的实现文件
// 负责从文件或文本流中读取文档
// ============================================================================

#include "io/DocumentReader.h"

/**
 * @brief 构造函数
 * 创建一个文档读取器
 */
DocumentReader::DocumentReader()
    : m_hasError(false)
{
}

/**
 * @brief 从文件读取文档
 * @param fileName 文件路径
 * @return 读取的文档，失败返回nullptr
 */
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

/**
 * @brief 从文本流读取文档
 * @param stream 文本流
 * @return 读取的文档，失败返回nullptr
 */
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

/**
 * @brief 检查是否有错误
 * @return 是否有错误
 */
bool DocumentReader::hasError() const
{
    return m_hasError;
}

/**
 * @brief 获取错误信息
 * @return 错误信息
 */
QString DocumentReader::errorString() const
{
    return m_errorString;
}

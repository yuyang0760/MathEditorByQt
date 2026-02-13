// ============================================================================
// DocumentWriter.cpp
// 文档写入器类的实现文件
// 负责将文档写入文件或文本流
// ============================================================================

#include "io/DocumentWriter.h"

/**
 * @brief 构造函数
 * 创建一个文档写入器
 */
DocumentWriter::DocumentWriter()
    : m_hasError(false)
{
}

/**
 * @brief 将文档写入文件
 * @param document 要写入的文档
 * @param fileName 文件路径
 * @return 是否写入成功
 */
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

/**
 * @brief 将文档写入文本流
 * @param document 要写入的文档
 * @param stream 文本流
 * @return 是否写入成功
 */
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
            stream << paragraph.plainText() << "\n";
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

/**
 * @brief 检查是否有错误
 * @return 是否有错误
 */
bool DocumentWriter::hasError() const
{
    return m_hasError;
}

/**
 * @brief 获取错误信息
 * @return 错误信息
 */
QString DocumentWriter::errorString() const
{
    return m_errorString;
}

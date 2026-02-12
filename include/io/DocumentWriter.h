// ============================================================================
// DocumentWriter.h
// 文档写入器类的头文件
// 负责将文档写入文件或文本流
// ============================================================================

#ifndef DOCUMENTWRITER_H
#define DOCUMENTWRITER_H

#include "core/Document.h"
#include <QFile>
#include <QTextStream>
#include <QString>

/**
 * @class DocumentWriter
 * @brief 文档写入器类
 * 
 * 负责将文档写入文件或文本流。
 * 提供了将文档写入文件和文本流的方法，并支持错误处理。
 */
class DocumentWriter
{
public:
    /**
     * @brief 构造函数
     * 创建一个文档写入器
     */
    DocumentWriter();
    
    /**
     * @brief 将文档写入文件
     * @param document 要写入的文档
     * @param fileName 文件路径
     * @return 是否写入成功
     */
    bool write(const Document *document, const QString &fileName);
    
    /**
     * @brief 将文档写入文本流
     * @param document 要写入的文档
     * @param stream 文本流
     * @return 是否写入成功
     */
    bool write(const Document *document, QTextStream &stream);
    
    /**
     * @brief 检查是否有错误
     * @return 是否有错误
     */
    bool hasError() const;
    
    /**
     * @brief 获取错误信息
     * @return 错误信息
     */
    QString errorString() const;
    
private:
    /**
     * @brief 是否有错误
     */
    bool m_hasError;
    
    /**
     * @brief 错误信息
     */
    QString m_errorString;
};

#endif // DOCUMENTWRITER_H

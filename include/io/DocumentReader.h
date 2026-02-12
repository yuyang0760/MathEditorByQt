// ============================================================================
// DocumentReader.h
// 文档读取器类的头文件
// 负责从文件或文本流中读取文档
// ============================================================================

#ifndef DOCUMENTREADER_H
#define DOCUMENTREADER_H

#include "core/Document.h"
#include <QFile>
#include <QTextStream>
#include <QString>

/**
 * @class DocumentReader
 * @brief 文档读取器类
 * 
 * 负责从文件或文本流中读取文档。
 * 提供了从文件和文本流读取文档的方法，并支持错误处理。
 */
class DocumentReader
{
public:
    /**
     * @brief 构造函数
     * 创建一个文档读取器
     */
    DocumentReader();
    
    /**
     * @brief 从文件读取文档
     * @param fileName 文件路径
     * @return 读取的文档，失败返回nullptr
     */
    Document *read(const QString &fileName);
    
    /**
     * @brief 从文本流读取文档
     * @param stream 文本流
     * @return 读取的文档，失败返回nullptr
     */
    Document *read(QTextStream &stream);
    
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

#endif // DOCUMENTREADER_H

#ifndef DOCUMENTREADER_H
#define DOCUMENTREADER_H

#include "core/Document.h"
#include <QFile>
#include <QTextStream>

class DocumentReader
{
public:
    DocumentReader();
    
    // 从文件读取文档
    Document *read(const QString &fileName);
    
    // 从文本流读取文档
    Document *read(QTextStream &stream);
    
    // 检查是否有错误
    bool hasError() const;
    QString errorString() const;
    
private:
    bool m_hasError;
    QString m_errorString;
};

#endif // DOCUMENTREADER_H

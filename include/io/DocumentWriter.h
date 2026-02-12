#ifndef DOCUMENTWRITER_H
#define DOCUMENTWRITER_H

#include "core/Document.h"
#include <QFile>
#include <QTextStream>

class DocumentWriter
{
public:
    DocumentWriter();
    
    // 将文档写入文件
    bool write(const Document *document, const QString &fileName);
    
    // 将文档写入文本流
    bool write(const Document *document, QTextStream &stream);
    
    // 检查是否有错误
    bool hasError() const;
    QString errorString() const;
    
private:
    bool m_hasError;
    QString m_errorString;
};

#endif // DOCUMENTWRITER_H

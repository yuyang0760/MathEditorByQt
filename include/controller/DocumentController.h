// ============================================================================
// DocumentController.h
// 文档控制器类的头文件
// 负责处理文档的增删改查操作，是模型和视图之间的桥梁
// ============================================================================

#ifndef DOCUMENTCONTROLLER_H
#define DOCUMENTCONTROLLER_H

#include "core/Document.h"
#include "core/Selection.h"
#include <QObject>

/**
 * @class DocumentController
 * @brief 文档控制器类
 * 
 * 负责处理文档的增删改查操作，是模型和视图之间的桥梁。
 * 提供了文本插入、删除、替换，段落插入、删除、合并等操作。
 */
class DocumentController : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    DocumentController(QObject *parent = nullptr);
    
    /**
     * @brief 设置文档
     * @param document 要设置的文档
     */
    void setDocument(Document *document);
    
    /**
     * @brief 获取文档
     * @return 当前文档
     */
    Document *document() const;
    
    /**
     * @brief 在指定位置插入文本
     * @param position 插入位置
     * @param text 要插入的文本
     */
    void insertText(const Selection::Position &position, const QString &text);
    
    /**
     * @brief 删除选中的文本
     * @param selection 选中的文本范围
     */
    void deleteText(const Selection &selection);
    
    /**
     * @brief 替换选中的文本
     * @param selection 选中的文本范围
     * @param text 替换的文本
     */
    void replaceText(const Selection &selection, const QString &text);
    
    /**
     * @brief 在指定位置插入段落
     * @param paragraphIndex 插入位置
     */
    void insertParagraph(int paragraphIndex);
    
    /**
     * @brief 删除指定段落
     * @param paragraphIndex 要删除的段落索引
     */
    void deleteParagraph(int paragraphIndex);
    
    /**
     * @brief 合并指定段落和下一段落
     * @param paragraphIndex 要合并的段落索引
     */
    void mergeParagraphs(int paragraphIndex);
    
    /**
     * @brief 应用格式到选中的文本
     * @param selection 选中的文本范围
     * @param format 要应用的格式
     */
    void applyFormat(const Selection &selection, const Format &format);
    
signals:
    /**
     * @brief 文档发生变化的信号
     * 当文档内容发生变化时发出此信号
     */
    void documentChanged();
    
private:
    /**
     * @brief 当前文档
     */
    Document *m_document;
};

#endif // DOCUMENTCONTROLLER_H

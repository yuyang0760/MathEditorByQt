// ============================================================================
// DocumentController.h
// 文档控制器头文件
// 提供文档内容的编辑操作接口
// ============================================================================

#ifndef DOCUMENTCONTROLLER_H
#define DOCUMENTCONTROLLER_H

#include "core/Document.h"
#include "core/Selection.h"
#include "core/StyleManager.h"
#include "core/CharacterFormat.h"
#include <QObject>

// 前向声明
class DocumentView;

/**
 * @brief 文档控制器类
 * 
 * DocumentController类负责文档内容的编辑操作，提供文本插入、替换、删除等功能。
 * 它是连接用户输入和文档模型的桥梁，处理各种编辑命令并更新文档状态。
 */
class DocumentController : public QObject {
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针
     */
    DocumentController(QObject *parent = nullptr);

    /**
     * @brief 设置文档对象
     * @param document 要操作的文档指针
     */
    void setDocument(Document *document);
    
    /**
     * @brief 获取当前文档对象
     * @return 当前文档指针
     */
    Document *document() const;

    /**
     * @brief 在指定位置插入文本
     * @param position 插入位置
     * @param text 要插入的文本
     */
    void insertText(const Position &position, const QString &text);
    
    /**
     * @brief 替换选择区域的文本
     * @param selection 选择区域
     * @param text 替换的文本
     */
    void replaceText(const Selection &selection, const QString &text);
    
    /**
     * @brief 删除选择区域的文本
     * @param selection 选择区域
     */
    void deleteText(const Selection &selection);
    
    /**
     * @brief 在指定位置插入段落
     * @param paragraphIndex 段落插入位置
     */
    void insertParagraph(int paragraphIndex);
    
    /**
     * @brief 在指定位置分割段落
     * @param position 分割位置
     * @return 新段落的索引
     */
    int splitParagraphAtPosition(const Position &position);
    
    /**
     * @brief 设置文档视图
     * @param view 文档视图指针
     */
    void setDocumentView(DocumentView *view);

    // 当前格式（直接格式）管理
    CharacterFormat currentDirectFormat() const;
    void setCurrentDirectFormat(const CharacterFormat &format);

    // 应用样式到选中区域
    void applyStyle(const Selection &selection, const QString &styleId);

    // 应用直接格式到选中区域
    void applyDirectFormat(const Selection &selection, const CharacterFormat &format);
    void applyDirectFormatToParagraph(Paragraph &para, int itemStart, int offsetStart,
                                      int itemEnd, int offsetEnd, const CharacterFormat &format);

signals:
    /**
     * @brief 文档变化信号
     * 当文档内容发生变化时发出
     */
    void documentChanged();

    /**
     * @brief 当前格式变化信号
     * @param format 新的当前格式
     */
    void currentFormatChanged(const CharacterFormat &format);

private:
    Document *m_document;
    CharacterFormat m_currentDirectFormat;   // 当前直接格式（用于新插入文本）
    StyleManager *m_styleMgr;       // 样式管理器指针
    DocumentView *m_documentView;   // 文档视图指针

    // 辅助函数
    Paragraph::Item createTextItem(const TextRun &run);
    void applyStyleToParagraph(Paragraph &para, int itemStart, int offsetStart,
                               int itemEnd, int offsetEnd, const QString &styleId);
};

#endif // DOCUMENTCONTROLLER_H

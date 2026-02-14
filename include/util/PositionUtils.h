// ============================================================================
// PositionUtils.h
// 位置工具类头文件
// 提供文档位置和字符信息的计算功能
// ============================================================================

#ifndef POSITIONUTILS_H
#define POSITIONUTILS_H

#include <QPointF>
#include "../core/Document.h"
#include "../core/Paragraph.h"
#include "../core/TextRun.h"
#include "../core/Selection.h"

class DocumentView;

/**
 * @brief 字符信息结构
 * 
 * CharInfo结构体存储单个字符的详细信息，包括字符本身、位置、宽度等。
 * 用于光标定位和字符级操作。
 */
struct CharInfo {
    bool valid = false;         ///< 信息是否有效
    QChar ch;                  ///< 字符内容
    int currentLineIndex = -1;  ///< 当前行内的字符索引
    QPointF pos;               ///< 字符位置
    qreal width = 0;            ///< 字符宽度
};

/**
 * @brief 位置工具类
 * 
 * PositionUtils类提供文档位置和字符信息的计算功能，是处理文档位置的工具类。
 * 它包含静态方法，用于字符索引计算、字符信息获取等操作。
 * 
 * 主要功能：
 * - 计算段落内的字符索引
 * - 获取字符详细信息
 * - 位置转换和计算
 */
class PositionUtils {
public:
    /**
     * @brief 计算当前段落内的字符索引
     * @param pos 文档位置
     * @param document 文档对象
     * @return 段落内的字符索引
     */
    static int currentParagraphCharIndex(const Position &pos, Document *document);
    
    /**
     * @brief 获取字符信息
     * @param pos 文档位置
     * @param document 文档对象
     * @param view 文档视图
     * @param isLeft 是否获取左侧字符
     * @return 字符信息结构体
     */
    static CharInfo getCharInfo(const Position &pos, Document *document, DocumentView *view, bool isLeft);
};

#endif // POSITIONUTILS_H
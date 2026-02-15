// ============================================================================
// PropertyState.h
// 属性状态类头文件
// 定义格式属性的三种状态：未设置、设置为true、设置为false
// ============================================================================

#ifndef PROPERTYSTATE_H
#define PROPERTYSTATE_H

/**
 * @brief 属性状态枚举
 * 
 * 表示格式属性的三种状态：
 * - Unset: 属性未设置，使用继承值
 * - SetTrue: 属性已设置为true
 * - SetFalse: 属性已设置为false
 */
enum class PropertyState {
    Unset,
    SetTrue,
    SetFalse
};

#endif // PROPERTYSTATE_H

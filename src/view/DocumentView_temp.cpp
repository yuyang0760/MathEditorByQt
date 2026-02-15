/**
 * @brief 更新布局
 */
void DocumentView::updateLayout() {
    // 保存当前视图中心点（场景坐标）
    QPointF center = mapToScene(viewport()->rect().center());
    
    clearLayouts();
    rebuildScene();
    
    // 恢复视图中心
    centerOn(center);
    ensureCursorVisible();
}
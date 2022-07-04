import QtQuick
import QtQuick.Layouts

Item {
    id: layout

    property int columns: 1
    property int rows: 1

    onChildrenChanged: updatePreferredSizes()
    onWidthChanged: updatePreferredSizes()
    onHeightChanged: updatePreferredSizes()
    onColumnsChanged: updatePreferredSizes()
    onRowsChanged: updatePreferredSizes()

    function updatePreferredSizes()
    {
        if(layout.children.length === 0)
        {
            return
        }

        var cellWidth = layout.width / columns;
        var cellHeight = layout.height / rows;
        for(var i=0;i<layout.children.length;++i)
        {
            var obj = layout.children[i]

            var c = obj.Layout.column
            var r = obj.Layout.row
            var cs = obj.Layout.columnSpan
            var rs = obj.Layout.rowSpan

            obj.x = c * cellWidth;
            obj.y = r * cellHeight;
            obj.height = cs * cellHeight;
            obj.width = rs * cellWidth;
        }

    }
}

import QtQuick 2.0
import QtQuick.Controls 2.2

Rectangle {
    property alias url: image.source
    Image{
        id:image
        width: 88; height: 78
    }
}

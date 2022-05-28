#include "editor/view.h"
#include "editor/scene.h"
#include "editor/items/flowchartshapeitems.h"
#include "editor/items/flowcharttextitems.h"
#include "editor/connectoritem.h"
#include "editor/grid.h"
#include "ui/shapeitemdrag.h"
#include "editor/undo_commands/undocommands.h"

#include <QWheelEvent>
#include <QTimeLine>
#include <QLabel>
#include <QToolButton>
#include <QBoxLayout>
#include <QUndoStack>
#include <QAction>
#include <QDebug>

const QString CursorPositionText            = QObject::tr("Cursor position: (%1, %2)");
const QString SelectedItemPositionText      = QObject::tr("Item position: (%1, %2)");
const QString SelectedItemFigureTypeText    = QObject::tr("TO REMOVE!!!");

const qreal MaxScale  = 10.0;
const qreal MinScale  = 0.25;
const qreal ScaleStep = 0.15;

View::View(Scene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
    , scene_(scene)
    , cursorPositionLabel_(new QLabel(CursorPositionText.arg("0").arg("0")))
    , selectedItemFigureTypeLabel_(new QLabel(SelectedItemFigureTypeText))
    , selectedItemPositionLabel_(new QLabel(SelectedItemPositionText))
    , undoStack_(new QUndoStack(this))
{
    init();
}

View::~View()
{
    qDebug() << "View: Destructor";
}

void View::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter, rect);

    if (Grid::enabled()) {
        const int gridSize = Grid::gridSize();

        int left = int(rect.left() - (int(rect.left()) % gridSize));
        int top  = int(rect.top()  - (int(rect.top())  % gridSize));

        QVarLengthArray<QLineF, 100> lines; // It works faster than reserving. IDK why
        for (int x = left; x < rect.right(); x += gridSize) {
            lines.append(QLineF(x, rect.top(), x, rect.bottom()));
        }
        for (int y = top; y < rect.bottom(); y += gridSize) {
            lines.append(QLineF(rect.left(), y, rect.right(), y));
        }

        painter->setPen(QPen(gridColor_, 0.5));
        painter->drawLines(lines.data(), lines.size());
    }
}

void View::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::CTRL) {
        if (event->delta() > 0) {
            zoomIn();
        } else {
            zoomOut();
        }

    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void View::mousePressEvent(QMouseEvent *event)
{
    if (event->modifiers() == Qt::CTRL || event->button() == Qt::MiddleButton) {
        setDragMode(ScrollHandDrag);
        setInteractive(false);
        QMouseEvent leftClick(QEvent::MouseButtonPress, event->pos(), Qt::LeftButton,
                              event->buttons(), Qt::KeyboardModifiers());
        QGraphicsView::mousePressEvent(&leftClick);

    } else {
        if (event->modifiers() == Qt::SHIFT) {
            setDragMode(RubberBandDrag);
        }

        QGraphicsView::mousePressEvent(event);
    }
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    setDragMode(NoDrag);
    setInteractive(true);
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    updateCursorPositionLabel(event->pos());
    QGraphicsView::mouseMoveEvent(event);
}

void View::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsView::mouseDoubleClickEvent(event);
    if (itemAt(event->pos()) == nullptr) {        
        QPointF pos = mapToScene(event->pos());

        AddCommand* addCommand = AddCommand::fromTextItem("", pos, scene_);
        undoStack_->push(addCommand);
    }
}

void View::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "View::dragEnterEvent: mime type:" << ShapeItemMimeData::mimeType();
    if (event->mimeData()->hasFormat(ShapeItemMimeData::mimeType()))
        event->acceptProposedAction();
}

void View::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void View::dropEvent(QDropEvent *event)
{
    const auto* shapeItemMimeData = qobject_cast<const ShapeItemMimeData*>(event->mimeData());
    if (shapeItemMimeData) {
        int itemType = shapeItemMimeData->itemType();
        QPointF pos = mapToScene(event->pos());

        for (auto item : scene_->selectedItems()) {
            item->setSelected(false);
        }

        undoStack_->push(AddCommand::fromShapeItem(itemType, pos, scene_));
    }
}

void View::addToUndoStackMoveCommand(QGraphicsItem *item, const QPointF &oldPos)
{
    if (auto shapeItem = dynamic_cast<FlowchartShapeItem*>(item)) {
        undoStack_->push(MoveCommand::fromShapeItem(shapeItem, oldPos));
    } else if (auto textItem = dynamic_cast<FlowchartTextItem*>(item)) {
        undoStack_->push(MoveCommand::fromTextItem(textItem, oldPos));
    }
}

void View::addToUndoStackAddConnectorCommand(ConnectorItem *connectorItem)
{
    undoStack_->push(AddCommand::fromConnectorItem(connectorItem, scene_));
}

void View::addToUndoStackResizeCommand(FlowchartShapeItem *resizedItem,
                                       const QRectF &oldRect,
                                       const QRectF &newRect,
                                       const QPointF &posBeforeResize,
                                       const QPointF &posAfterResize)
{
    undoStack_->push(new ResizeCommand(resizedItem, oldRect, newRect, posBeforeResize, posAfterResize));
}

void View::updateGridColor(const QColor &color)
{
    gridColor_ = color;
    viewport()->update();
}

void View::setBackgroundEnabled(bool enabled)
{
    setBackgroundBrush(enabled ? backgroundColor_ : Qt::white);
}

void View::updateBackgroundColor(const QColor &color)
{
    backgroundColor_ = color;
    setBackgroundBrush(backgroundColor_);
}

void View::zoom(qreal zoomRatio)
{
    qreal currentScaleRatio = transform().m11();
    qreal scale = zoomRatio / currentScaleRatio;
    this->scale(scale, scale);
    emit scaleChanged(transform().m11());
}

void View::deleteSelectedItem()
{
    auto selectedItems = scene_->selectedItems();
    QGraphicsItem* selectedItem = (selectedItems.empty() ? nullptr : selectedItems.first());

    if (selectedItem) {
        DeleteCommand* deleteCommand = nullptr;

        if (auto shapeItem = dynamic_cast<FlowchartShapeItem*>(selectedItem)) {
            deleteCommand = DeleteCommand::fromShapeItem(shapeItem, scene_);
        } else if (auto textItem = dynamic_cast<FlowchartTextItem*>(selectedItem)) {
            deleteCommand = DeleteCommand::fromTextItem(textItem, scene_);
        } else if (auto connectorItem = dynamic_cast<ConnectorItem*>(selectedItem)) {
            deleteCommand = DeleteCommand::fromConnectorItem(connectorItem, scene_);
        }

        if (deleteCommand)
            undoStack_->push(deleteCommand);
    }
}

void View::showAndUpdateItemInfoLabels(FlowchartShapeItem *selectedItem)
{
    selectedItemPositionLabel_->show();
    selectedItemFigureTypeLabel_->show();
    updateItemInfoLabels(selectedItem);
}

void View::hideItemInfoLabels()
{
    selectedItemPositionLabel_->hide();
    selectedItemFigureTypeLabel_->hide();
}

void View::updateItemInfoLabels(FlowchartShapeItem *selectedItem)
{
    updateSelectedItemFigureTypeLabel(selectedItem);
    updateSelectedItemPositionLabel(selectedItem);
}

void View::init()
{
    initFlags();
    initFont();
    initLayout();
    initActions();
    initConnection();
}

void View::initFlags()
{
    setMouseTracking(true);
    setAcceptDrops(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setTransformationAnchor(AnchorUnderMouse);
    setRenderHint(QPainter::Antialiasing);
}

void View::initFont()
{
    QFont font = this->font();
    font.setPointSize(12);
    setFont(font);
}

void View::initLayout()
{
    auto createHorizontalSpacer = [=]() {
        return new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
    };
    auto createVerticalSpacer = []() {
        return new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    };

    selectedItemFigureTypeLabel_->setAlignment(Qt::AlignRight);
    selectedItemFigureTypeLabel_->hide();

    selectedItemPositionLabel_->setAlignment(Qt::AlignRight);
    selectedItemPositionLabel_->hide();

    cursorPositionLabel_->setAlignment(Qt::AlignRight);

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addWidget(selectedItemFigureTypeLabel_);
    vLayout->addWidget(selectedItemPositionLabel_);
    vLayout->addWidget(new QLabel()); // Spacer
    vLayout->addWidget(cursorPositionLabel_);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);

    QGridLayout* gLayout = new QGridLayout;
    gLayout->addItem(createHorizontalSpacer(), 0, 0);
    gLayout->addItem(createVerticalSpacer(), 0, 1);
    gLayout->addLayout(vLayout, 1, 1);

    gLayout->setContentsMargins(0, 0, 0, 0);
    gLayout->setSpacing(0);

    setLayout(gLayout);
}

void View::initActions()
{
    undoAction_ = undoStack_->createUndoAction(this, tr("&Undo"));
    undoAction_->setShortcut(QKeySequence::Undo);
    this->addAction(undoAction_);

    redoAction_ = undoStack_->createRedoAction(this, tr("&Redo"));
    redoAction_->setShortcut(QKeySequence::Redo);
    this->addAction(redoAction_);
}

void View::initConnection()
{
    connect(scene_, &Scene::itemSelected, this, &View::showAndUpdateItemInfoLabels);

    connect(scene_, &Scene::itemMoved, this, &View::updateItemInfoLabels);

    connect(scene_, &Scene::itemLostSelection, this, &View::hideItemInfoLabels);
}

void View::zoomIn()
{
    qreal currentScale = transform().m11();
    qreal resultScale = currentScale + ScaleStep;
    resultScale = qMin(resultScale, MaxScale);
    qreal scale = resultScale / currentScale;
    this->scale(scale, scale);
    emit scaleChanged(resultScale);
}

void View::zoomOut()
{
    qreal currentScale = transform().m11();
    qreal resultScale = currentScale - ScaleStep;
    resultScale = qMax(resultScale, MinScale);
    qreal scale = resultScale / currentScale;
    this->scale(scale, scale);
    emit scaleChanged(resultScale);
}

void View::updateSelectedItemPositionLabel(FlowchartShapeItem *selectedItem)
{
    QPointF pos = selectedItem->pos();
    selectedItemPositionLabel_->setText(SelectedItemPositionText.arg(pos.x()).arg(pos.y()));
}

void View::updateSelectedItemFigureTypeLabel(FlowchartShapeItem *selectedItem)
{
//    selectedItemFigureTypeLabel_->setText(SelectedItemFigureTypeText.arg(selectedItem->figureType()));
}

void View::updateCursorPositionLabel(const QPoint &pos)
{
    QPoint cursorPos = mapToScene(pos).toPoint();
    cursorPositionLabel_->setText(CursorPositionText.arg(cursorPos.x()).arg(cursorPos.y()));
}

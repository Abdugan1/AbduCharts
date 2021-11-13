#include "view.h"
#include "scene.h"
#include "myitem.h"

#include <QWheelEvent>
#include <QTimeLine>
#include <QLabel>
#include <QBoxLayout>
#include <QDebug>

const QString cursorPositionText = QObject::tr("Cursor position: (%1, %2)");
const QString selectedItemPositionText = QObject::tr("Item position: (%1, %2)");

View::View(Scene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
    , scene_(scene)
    , cursorPositionLabel_(new QLabel(cursorPositionText.arg("0").arg("0")))
    , selectedItemPositionLabel_(new QLabel(selectedItemPositionText))
{
    init();
}

void View::drawBackground(QPainter *painter, const QRectF &rect)
{
    constexpr int gridSize = 20;
    int left = int(rect.left() - (int(rect.left()) % gridSize));
    int top  = int(rect.top()  - (int(rect.top())  % gridSize));

    QVarLengthArray<QPoint> points; // It works faster than reserving. IDK why
    for (int x = left; x < rect.right(); x += gridSize) {
        for (int y = top; y < rect.bottom(); y += gridSize) {
            points.append(QPoint(x, y));
        }
    }

    painter->drawPoints(points.data(), points.size());

    QGraphicsView::drawBackground(painter, rect);
}

void View::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::CTRL) {
        int numDegrees = event->delta() / 8;
        qDebug() << "numDegrees:" << numDegrees;
        int numSteps = numDegrees / 15;
        numScheduledScalings_ += numSteps;
        if (numScheduledScalings_ * numSteps < 0)
            numScheduledScalings_ = numSteps;

        QTimeLine* anim = new QTimeLine(350, this);
        anim->setUpdateInterval(20);
        connect(anim, &QTimeLine::valueChanged, this, &View::scalingTime);
        connect(anim, &QTimeLine::finished, this, &View::animFinished);
        anim->start();

    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    QPoint cursorPos = mapToScene(event->pos()).toPoint();
    cursorPositionLabel_->setText(cursorPositionText.arg(cursorPos.x()).arg(cursorPos.y()));
    QGraphicsView::mouseMoveEvent(event);
}

void View::scalingTime(qreal x)
{
    qreal factor = 1.0f + qreal(numScheduledScalings_) / 300.0f;
//    qDebug() << "factor:" << factor;
    scale(factor, factor);
}

void View::animFinished()
{
    if (numScheduledScalings_ > 0) {
        --numScheduledScalings_;
    } else {
        ++numScheduledScalings_;
        sender()->deleteLater();
    }
}

void View::init()
{
    initFlags();
    initSomething();
    initLayout();
    initConnection();
}

void View::initFlags()
{
    setViewportUpdateMode(FullViewportUpdate);
    setMouseTracking(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void View::initSomething()
{
    QFont font = this->font();
    font.setPointSize(14);
    setFont(font);
}

void View::initLayout()
{
    selectedItemPositionLabel_->setAlignment(Qt::AlignRight);
    selectedItemPositionLabel_->hide();
    cursorPositionLabel_->setAlignment(Qt::AlignRight);

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addWidget(selectedItemPositionLabel_);
    vLayout->addWidget(cursorPositionLabel_);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);

    QSpacerItem* hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem* vSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

    QGridLayout* gLayout = new QGridLayout;
    gLayout->addItem(hSpacer, 0, 0);
    gLayout->addItem(vSpacer, 0, 1);
    gLayout->addLayout(vLayout, 1, 1);

    gLayout->setContentsMargins(0, 0, 0, 0);
    gLayout->setSpacing(0);

    setLayout(gLayout);
}

void View::initConnection()
{
    connect(scene_, &Scene::itemSelected, selectedItemPositionLabel_, &QLabel::show);

    connect(scene_, &Scene::itemMoved, this, &View::updateSelectedItemPositionLabel);

    connect(scene_, &Scene::itemLostSelection, selectedItemPositionLabel_, &QLabel::hide);
}

void View::updateSelectedItemPositionLabel(MyItem *selectedItem)
{
    QPointF pos = selectedItem->pos();
    selectedItemPositionLabel_->setText(selectedItemPositionText.arg(pos.x()).arg(pos.y()));
}

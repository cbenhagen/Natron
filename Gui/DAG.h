//  Powiter
//
//  Created by Alexandre Gauthier-Foichat on 06/12
//  Copyright (c) 2013 Alexandre Gauthier-Foichat. All rights reserved.
//  contact: immarespond at gmail dot com
#ifndef NODEGRAPH_H
#define NODEGRAPH_H
#include <cmath>
#include <QtWidgets/QGraphicsView>
#include <iostream>
#include <QtCore/QEvent>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QInputDialog>
#include <boost/noncopyable.hpp>
#include "Superviser/powiterFn.h"

class QVBoxLayout;
class Node;
class NodeGui;
class Controler;
class Edge;
class SmartInputDialog;
class SettingsPanel;

class NodeGraph: public QGraphicsView , public boost::noncopyable{
    enum EVENT_STATE{DEFAULT,MOVING_AREA,ARROW_DRAGGING,NODE_DRAGGING};
    Q_OBJECT

public:

    NodeGraph(QGraphicsScene* scene=0,QWidget *parent=0);

    virtual ~NodeGraph();
 
    void createNodeGUI(QVBoxLayout *dockContainer,Powiter_Enums::UI_NODE_TYPE type,Node *node,double x,double y);
    
    void removeNode(NodeGui* n);

    virtual void enterEvent(QEvent *event);
    
    virtual void leaveEvent(QEvent *event);

    virtual void keyPressEvent(QKeyEvent *e);
    
    virtual bool event(QEvent* event);
    
    void autoConnect(NodeGui* selected,NodeGui* created);
    
    void setSmartNodeCreationEnabled(bool enabled){smartNodeCreationEnabled=enabled;}

    void checkIfViewerConnectedAndRefresh(NodeGui* n);
    
    void selectNode(NodeGui* n);
    
    void deselect();
protected:

    void mousePressEvent(QMouseEvent *event);
    
    void mouseReleaseEvent(QMouseEvent *event);
    
    void mouseMoveEvent(QMouseEvent *event);
    
    void mouseDoubleClickEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

    void scaleView(qreal scaleFactor,QPointF center);

private:
    
    void deleteSelectedNode();
    
    bool smartNodeCreationEnabled;
    QPointF old_pos;
    QPointF oldp;
    QPointF oldZoom;
    QPointF _lastSelectedPos;
    EVENT_STATE _evtState;
    NodeGui* _nodeSelected;
    Edge* _arrowSelected;
    std::vector<NodeGui*> _nodes;
    bool _nodeCreationShortcutEnabled;
    bool _fullscreen;
    QGraphicsItem* _root;
    
};


#endif // NODEGRAPH_H

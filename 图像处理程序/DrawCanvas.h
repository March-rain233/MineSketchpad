#pragma once
#include <QWidget>
#include <QMouseEvent>
#include <QStack.h>
#include <QVector.h>
#include <QPainter>
#include "ui_DrawCanvas.h"
#include "DrawCommand.h"
#include "Image.h"
#include "DrawTools.h"

class DrawTools;

/// <summary>
/// ��ͼ�����
/// </summary>
class DrawCanvas : public QWidget {
	Q_OBJECT

public:
	DrawCanvas(QWidget* parent = Q_NULLPTR);
	~DrawCanvas();

	void Undo();// ���ز���
	void Redo();// ����ִ�в���
	void ClearCommand();//���������¼
	void ClearRedoCommand();//������������Ĳ�����¼

	void AddLayer(const MyImage::Image&);//����ͼ��
	void InsertLayer(const MyImage::Image&, int);//��ָ��λ�ò���ͼ��
	void DeleteLayer(int);//ɾ��ָ��ͼ��
	const QVector<MyImage::Image*>& GetLayers();//��ȡͼ����
	QVector<int>& GetSelected();//��ȡ��ǰѡ�е�ͼ������

	void SetScale(float);//����������
	float GetScale();//���������

	QPoint GetDrawPoint();//��û滭��ʼ��
	void SetDrawPoint(QPoint);

	void SaveImage(const QString&);// �����ļ�

	void PushCommand(DrawCommand*);//������ִ������

	void SetTool(DrawTools*);//���û滭����

	void ClearCanvas();//�������
	bool IsEmpty();//�Ƿ��ڹ���
private:
	void mousePressEvent(QMouseEvent*) override;//��д��갴���¼�
	void mouseReleaseEvent(QMouseEvent*) override;//��д����ͷ��¼�
	void mouseMoveEvent(QMouseEvent*) override;//��д����ƶ��¼�
	void contextMenuEvent(QContextMenuEvent*) override;//��д�˵��¼�
	void keyPressEvent(QKeyEvent* e) override; //��д�����¼�
	void wheelEvent(QWheelEvent* event) override;//�����¼�

	void resizeEvent(QResizeEvent*) override;//��д��С�仯�¼�

	bool eventFilter(QObject* watched, QEvent* event) override;//��д�¼�������
	void DrawImage();//����ͼƬ

	void HorizontalMove(int);//��Ⱦλ�ú����ƶ�
	void VertiaclMove(int);//��Ⱦλ�������ƶ�

	int GetDrawHeight();//��ȡͼƬ��Ⱦ��ĸ߶�
	int GetDrawWidth();//��ȡͼƬ��Ⱦ��Ŀ��

	QRect GetDrawRect();//��ȡ�������ʾ�������Ⱦ�������
private:
	Ui::DrawCanvas _ui;

	QStack<DrawCommand*> _historyCommand;//��ʷ����
	QStack<DrawCommand*> _redoCommand;//���������Ĳ���

	QVector<MyImage::Image*> _layers;//ͼ��

	DrawTools* _tool;//��ǰʹ�õĻ�ͼ����

	QPoint _drawPoint;//��Ⱦ��㣨���Ͻǣ�
	QImage* _canvasFill;//�����������
	const float _maxSizeRate = 3200;//�����Ŵ���
	const float _minSizeRate = 1;//�������С��
	float _scale = 1;//��ǰ������

	QVector<int> _seletcted;//��ǰѡ�е�ͼ��
};

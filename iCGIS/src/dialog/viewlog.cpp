#include "viewlog.h"

#include <QFileInfo>
#include <QFile>
#include <QHBoxLayout>
#include <QIcon>
#include <QTextCharFormat>

ViewLog::ViewLog(QWidget *parent)
	: QDialog(parent), logFileInfo("logs/basic_logger.txt")
{
	this->setWindowTitle(tr("View Log"));
	this->setWindowIcon(QIcon("res/icons/log.ico"));
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	setupLayout();

	levelColor[0].set("critical]", Qt::red);
	levelColor[1].set("error]", QColor(200, 0, 0));
	levelColor[2].set("warning]", Qt::blue);
	levelColor[3].set("info]", QColor(207,204,41));
	levelColor[4].set("debug]", QColor(90, 180, 90));
	levelColor[5].set("trace]", QColor(90, 220, 90));

	lastModifiedTime = QDateTime::currentDateTime();
	readLogFile();

	timer = new QTimer(this);
	timer->start(500);
	connect(timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
}

ViewLog::~ViewLog()
{
}


// 布局
void ViewLog::setupLayout()
{
	logText = new QTextEdit();
	logText->setReadOnly(true);
	logText->setFontFamily("Consolas");
	logText->setFontPointSize(12);
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->addWidget(logText);
}


// 高亮 Error、warning、trace、info、debug等日志级别
void ViewLog::highlightText()
{
	QTextDocument* document = logText->document();

	// 6个日志级别
	for (int i = 0; i < 6; ++i) {
		QTextCursor highlight_cursor(document);
		QTextCharFormat color_format(highlight_cursor.charFormat());
		color_format.setForeground(levelColor[i].color);
		while (!highlight_cursor.isNull() && !highlight_cursor.atEnd()) {
			highlight_cursor = document->find(levelColor[i].level, highlight_cursor, QTextDocument::FindWholeWords);
			if (!highlight_cursor.isNull()) {
				highlight_cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
				highlight_cursor.mergeCharFormat(color_format);
			}
		}
	}
}


// 读取日志文件
void ViewLog::readLogFile()
{
	QFile file(logFilePath);
	if (!file.open(QFile::ReadOnly | QFile::Text))
		return;

	logText->setText(file.readAll());
	file.close();
	highlightText();
	logText->moveCursor(QTextCursor::End);
}

// slot of timer
// 每500ms检查一次是否有日志写入
void ViewLog::handleTimeout()
{
	QFileInfo fileInfo(logFilePath);
	QDateTime modifiedTime = fileInfo.lastModified();

	// 文件被修改
	if (modifiedTime != lastModifiedTime) {
		lastModifiedTime = modifiedTime;
		readLogFile();
	}
}


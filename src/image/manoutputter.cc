/*
 * File:   manoutputter.cc
 * Author: Christian Sciberras
 * Created: 20 May 2010
 *   This file is part of wkhtmltoimage.
 *   wkhtmltoimage is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *   wkhtmltoimage is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *   You should have received a copy of the GNU General Public License
 *   along with wkhtmltoimage.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "commandlineparser_p.hh"
#include <QStringList>
#define S(x) ((x).toUtf8().constData())

class ManOutputter: public Outputter {
private:
	FILE * fd;
	int order;
public:
	ManOutputter(FILE * _): fd(_) {
		fprintf(fd,".TH wkhtmltoimage 1 \"2010 May 21\"\n\n");
	}

	void beginSection(const QString & name) {
		fprintf(fd, ".SH %s\n", S(name));
	}

	void endSection() {
		fprintf(fd, "\n");
	}

	void beginParagraph() {
	}

	void endParagraph() {
		fprintf(fd, "\n\n");
	}
	
	void text(const QString & t) {
		QString str = QString(t).replace("-", "\\-");
		fprintf(fd, "%s", S(str));
	}
	
	void bold(const QString & t) {
		fprintf(fd, "\\fB%s\\fP", S(t));
	}
	
	void italic(const QString & t) {
		fprintf(fd, "\\fB%s\\fP", S(t));
	}

	void link(const QString & t) {
		fprintf(fd, "<%s>", S(t));
	}

	void verbatim(const QString & t) {
		QString str = QString(t).replace("-", "\\-");
		QStringList l = str.split('\n');
		while ( l.back() == "") l.pop_back();
		foreach(const QString & line, l)
			fprintf(fd, "  %s\n", S(line));
		fprintf(fd, "\n");
	}
	
	void beginSwitch() {
		fprintf(fd, ".PD 0\n");
	}

	void beginList(bool ordered) {
		order=(ordered?1:-1);
	}

	void endList() {
		fprintf(fd, "\n");
	}
	
	void listItem(const QString & s) {
		if (order < 0) fprintf(fd, " * ");
		else fprintf(fd, "%3d ", order++);
		fprintf(fd,"%s\n",S(s));
	}
	
	void cswitch(const ArgHandler * h) {
		fprintf(fd, ".TP\n");
		fprintf(fd, "\\fB");
		if(h->shortSwitch != 0)
			fprintf(fd, "\\-%c, ", h->shortSwitch);
		else
			fprintf(fd, "    ");
		fprintf(fd,"\\-\\-%s\\fR", S(h->longName));
		
		for(QVector<QString>::const_iterator i = h->argn.constBegin(); i != h->argn.constEnd(); ++i)
			fprintf(fd," \\fI<%s>\\fR", S(*i));
		
		fprintf(fd, "\n%s\n",S(h->desc));
	}
	
	void endSwitch() {
		fprintf(fd, ".PD\n");
		fprintf(fd, "\n");
	}
};

/*!
  Create a man page outputter
  \param fd A file description to output to
*/
Outputter * Outputter::man(FILE * fd) {
  return new ManOutputter(fd);
}
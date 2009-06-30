// ============================================================================
// Copyright Jean-Charles LAMBERT - 2007-2008                                  
// e-mail:   Jean-Charles.Lambert@oamp.fr                                      
// address:  Dynamique des galaxies                                            
//           Laboratoire d'Astrophysique de Marseille                          
//           P�le de l'Etoile, site de Ch�teau-Gombert                         
//           38, rue Fr�d�ric Joliot-Curie                                     
//           13388 Marseille cedex 13 France                                   
//           CNRS U.M.R 6110                                                   
// ============================================================================
// See the complete license in LICENSE and/or "http://www.cecill.info".        
// ============================================================================
/**
	@author Jean-Charles Lambert <jean-charles.lambert@oamp.fr>
 */
#ifndef GLNEMOGLOBJECTOSD_H
#define GLNEMOGLOBJECTOSD_H
#include <QObject>
#include "gltextobject.h"

namespace glnemo {
class GLWindow;
class GLTextObject;
class GLObjectOsd : public GLObject {
  Q_OBJECT
  public:
    GLObjectOsd(const int w, const int h,const QFont &f,
		const QColor &c=Qt::green, bool activated=TRUE);
   ~GLObjectOsd();

   enum OsdKeys {
     DataType  ,
     Title     ,
     Nbody     ,
     Time      ,
     Getdata   ,
     Zoom      ,
     Rot       ,
     Trans     ,
     Loading   ,
     Projection,
     n_OsdKeys
   };
   GLTextObject * Osd_text;

  public slots:
    void setWH(int width, int height);
    void setFont(const QFont);
    void setFont(const OsdKeys k, const QFont);
    void setText(const OsdKeys k, const QString text);
    void setText(const OsdKeys k, const int);
    void setText(const OsdKeys k, const float);
    void setText(const OsdKeys k, const float, const float, const float);
    void setTextColor(const OsdKeys k,const QColor );
    void keysToggle(const OsdKeys k);
    void keysActivate(const OsdKeys k, const bool status);
    void updateDisplay();
    void updateDisplay(const OsdKeys k);
    void display(GLWindow *);
    void updateColor(const QColor);
    
  private:
    static char * OsdText[n_OsdKeys];
    QFont font;
};

}

#endif
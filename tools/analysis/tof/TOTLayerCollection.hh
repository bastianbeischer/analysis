#ifndef TOTLayerCollection_hh
#define TOTLayerCollection_hh

#include "PlotCollection.hh"

class TOTLayer;

class TOTLayerCollection : 
  public PlotCollection
{
public:
  TOTLayerCollection(TOTLayer* plot);
  ~TOTLayerCollection();
private:
  
  
};

#endif /* TOTLayerCollection_hh */

#ifndef FieldManager_hh
#define FieldManager_hh

class MagneticField;

class FieldManager
{
public:
  ~FieldManager();
  static FieldManager* instance();
  
  void setField(const MagneticField*);
  const MagneticField* field() const;

private:
  FieldManager();
  
private:
  static FieldManager* s_instance;

  const MagneticField* m_field;
};

#endif /* FieldManager_hh */

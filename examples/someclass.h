#include "SOAField.h"
#include "SOASkin.h"
#include "SOAUtils.h"
#include "SOAView.h"

struct track {
  float x;
  float y;
  float z;
  float tx;
  float ty;
};

SOAFIELD_TRIVIAL( f_track, accessor_track, track );
SOASKIN_TRIVIAL( s_track, f_track );

struct fitres {
  float px;
  float py;
  float pz;
  int   q;
};

SOAFIELD_TRIVIAL( f_fitres, accessor_fitres, fitres );
SOASKIN_TRIVIAL( s_fitres, f_fitres );

struct fitqual {
  float chi2;
  int   dof;
};

SOAFIELD_TRIVIAL( f_fitqual, accessor_fitqual, fitqual );
SOASKIN_TRIVIAL( s_fitqual, f_fitqual );

SOASKIN_TRIVIAL( s_track_with_fitres, f_track, f_fitres );
SOASKIN_TRIVIAL( s_track_with_fitqual, f_track, f_fitqual );
SOASKIN_TRIVIAL( s_track_with_fitres_and_fitqual, f_track, f_fitres, f_fitqual );

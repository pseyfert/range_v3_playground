/*
 * Copyright (C) 2019  CERN for the benefit of the LHCb collaboration
 * Author: Paul Seyfert <pseyfert@cern.ch>
 *
 * This software is distributed under the terms of the GNU General Public
 * Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
 *
 * In applying this licence, CERN does not waive the privileges and immunities
 * granted to it by virtue of its status as an Intergovernmental Organization
 * or submit itself to any jurisdiction.
 */

#include "SOAContainer/SOAField.h"
#include "SOAContainer/SOASkin.h"
#include "SOAContainer/SOAUtils.h"
#include "SOAContainer/SOAView.h"

struct track {
  float x;
  float y;
  float z;
  float tx;
  float ty;
  bool operator==(const track& other) const { return &other == this; }
  bool operator!=(const track& other) const { return &other != this; }
};

SOAFIELD_TRIVIAL( f_track, accessor_track, track );
SOASKIN_TRIVIAL( s_track, f_track );

struct fitres {
  float px;
  float py;
  float pz;
  int   q;
  bool operator==(const fitres& other) const { return &other == this; }
  bool operator!=(const fitres& other) const { return &other != this; }
};

SOAFIELD_TRIVIAL( f_fitres, accessor_fitres, fitres );
SOASKIN_TRIVIAL( s_fitres, f_fitres );

struct fitqual {
  float chi2;
  int   dof;
  bool operator==(const fitqual& other) const { return &other == this; }
  bool operator!=(const fitqual& other) const { return &other != this; }
};

SOAFIELD_TRIVIAL( f_fitqual, accessor_fitqual, fitqual );
SOASKIN_TRIVIAL( s_fitqual, f_fitqual );

SOASKIN_TRIVIAL( s_track_with_fitres, f_track, f_fitres );
SOASKIN_TRIVIAL( s_track_with_fitqual, f_track, f_fitqual );
SOASKIN_TRIVIAL( s_track_with_fitres_and_fitqual, f_track, f_fitres, f_fitqual );

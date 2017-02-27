#include <iostream>
#include <vector>
#include <math.h>
#include <sstream>

/**
 * Trouver le point géographique le plus proche d'une position et renvoyer le nom du lieu associé
 */

using namespace std;

class Point {
public:
  double latitude;
  double longitude;

  Point() {
    latitude = longitude = 0;
  }

  Point(string p_lat, string p_lon) {
    // replace ',' by '.' to match floats
    int pos = (int)p_lat.find(',');
    p_lat[pos] = '.';
    pos = (int)p_lon.find(',');
    p_lon[pos] = '.';
  }

  static double distance(const Point& a, const Point& b);
};

double Point::distance(const Point& a, const Point& b) {
  double x = (b.longitude - a.longitude) * cos((a.latitude + b.latitude) / 2);
  double y = b.latitude - a.latitude;
  double d = sqrt(pow(x,2) + pow(y,2)) * 6371;
  return d;
}

class Defibrilateur {
public:
  Defibrilateur(string descr);
  string id, name, address, tel;
  Point loc;
};

Defibrilateur::Defibrilateur(string descr) {
  // parse the ';' separted string 'descr' into
  // a vector of fields
  vector<string> fields;
  stringstream ss(descr);
  string field;

  while (getline(ss, field, ';')) {
    fields.push_back(field);
  }

  // update instance variables
  this->id = fields[0];
  this->name = fields[1];
  this->address = fields[2];
  this->tel = fields[3];
  string longitude = fields[4];
  string latitude = fields[5];
  Point p(latitude, longitude);
  this->loc = p;
}

int main() {
  // get the user loc
  string u_long, u_lat;
  getline(cin, u_long);
  getline(cin, u_lat);
  Point user(u_lat, u_long);

  // get the nb of defibrillators
  int N;
  cin >> N;
  cin.ignore();
  // build the list of defibrillators
  vector<Defibrilateur *> d;

  for (int i = 0; i < N; i++) {
    string descr; // ';' separated string
    getline(cin, descr);
    d.push_back(new Defibrilateur(descr));
  }

  // find the nearest defibrillator and store its address
  double min = -1;
  string addr;
  for (int i = 0; i < N; i++) {
    double dist = Point::distance(user, d[i]->loc);
    if ((min == -1) or (dist < min)) {
      min = dist;
      addr = d[i]->name;
    }
  }

  cout << addr << endl;

  // clean
  for (int i = 0; i < N; i++)
    delete d[i];

  return 0;
}

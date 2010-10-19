#include "MatrixStraightLine.hh"

#include "Manager.hh"
#include "Strategy.hh"
#include "Track.hh"
#include "Parameters.hh"
#include "Hit.hh"

#include <TVector3.h>
#include <TMatrixD.h>

#include <iostream>
#include <cmath>

#include "millepede.h"

MatrixStraightLine::MatrixStraightLine() :
  Matrix()
{
}

MatrixStraightLine::~MatrixStraightLine()
{
}


// void MatrixStraightLine::FillMatrixFromTrack(Track* track)
// {
//   std::vector<Hit> hits = track->hits();
  
//   for (unsigned int i = 0; i < hits.size(); i++) {
//     Hit hit = hits.at(i);
//     TVector3 pos = hit.position();
//     Manager* mgr = Manager::GetInstance();
//     Parameters* parameters = mgr->GetParameters();

//     int uniqueLayer = hit.uniqueLayer();

//     if (hit.hybrid() == 33 && hit.sipm() == 3)
//       continue;
//     if (hit.hybrid() == 21 && hit.sipm() == 3)
//       continue;
//     if (hit.hybrid() == 36 && hit.sipm() == 3)
//       continue;
//     if (hit.hybrid() == 23 && hit.sipm() == 3)
//       continue;

//     // if (
//     //     (hit.hybrid() == 20 && hit.sipm() == 3) ||
//     //     (hit.hybrid() == 22 && hit.sipm() == 0) ||
//     //     (hit.hybrid() == 22 && hit.sipm() == 3) ||
//     //     (hit.hybrid() == 23 && hit.sipm() == 2) ||
//     //     (hit.hybrid() == 23 && hit.sipm() == 3) ||
//     //     (hit.hybrid() == 30 && hit.sipm() == 0) ||
//     //     (hit.hybrid() == 30 && hit.sipm() == 3) ||
//     //     (hit.hybrid() == 31 && hit.sipm() == 2) ||
//     //     (hit.hybrid() == 32 && hit.sipm() == 0) ||
//     //     (hit.hybrid() == 32 && hit.sipm() == 3) ||
//     //     (hit.hybrid() == 33 && hit.sipm() == 2) ||
//     //     (hit.hybrid() == 34 && hit.sipm() == 0) ||
//     //     (hit.hybrid() == 36 && hit.sipm() == 0) ||
//     //     (hit.hybrid() == 37 && hit.sipm() == 0)
//     //     )
//     //   continue;


//     int shiftIndex = parameters->ShiftParameterIndexForItem(hit.hybrid(), hit.sipm());
//     int rotIndex   = parameters->RotationParameterIndexForItem(hit.uniqueLayer(), hit.hybrid(), hit.sipm());

//     //    std::cout << "hybrid: " << hit.hybrid() << " --> index: " << shiftIndex << std::endl; 

//     float z0 = 0.0;
//     float fx = pos.x();
//     float fy = pos.y();
//     float fz = pos.z();
//     float k  = fz - z0;

//     float nom_angle = parameters->GetAngle(uniqueLayer);
//     bool useTangens = fabs(nom_angle) < M_PI/4. ? true : false;

//     float cotangens = 0., tangens = 0., angle = 0.;

//     if (useTangens) {
//       tangens = parameters->GetParameter(rotIndex);
//       angle = atan(tangens);
//     }
//     else {
//       cotangens = parameters->GetParameter(rotIndex);
//       if (cotangens != 0)
//         angle = atan(1./cotangens);
//       else
//         angle = M_PI/2.;
//     }
//     //    std::cout << "layer: " << uniqueLayer << " --> angle: " << angle << std::endl;
//     // specify resolution
//     double sigmaV = hit.resolution();

//     // Rot is the matrix that maps u,v, to x,y (i.e. the backward rotation)
//     TMatrixD Rot(2,2); 
//     Rot(0,0) = cos(angle);
//     Rot(0,1) = sin(angle);
//     Rot(1,0) = -sin(angle);
//     Rot(1,1) = cos(angle);
//     TMatrixD V1(2,2);
//     V1(0,0) = 0.;
//     V1(0,1) = 0.;
//     V1(1,0) = 0.;
//     V1(1,1) = sigmaV*sigmaV;
//     TMatrixD RotTrans(2,2);
//     RotTrans.Transpose(Rot);
//     TMatrixD V2(2,2);
//     V2 = Rot * V1 * RotTrans;
//     // TMatrixD Lin(1,2);
//     // if (useTangens) {
//     //   Lin(0,0) = -tangens;
//     //   Lin(0,1) = 1.;
//     // }
//     // else {
//     //   Lin(0,0) = 1.;
//     //   Lin(0,1) = -cotangens;
//     // }
//     // TMatrixD LinTrans(2,1);
//     // LinTrans.Transpose(Lin);
//     // TMatrixD V3 = TMatrixD(1,1);
//     // V3 = Lin * V2 * LinTrans;
//     // float sigma = sqrt(V3(0,0));

//     float y,sigma;
//     if (useTangens) {
//       y = fy;
//       sigma = sqrt(V2(1,1));

//       // hardcoded for testbeam now, change -tangens to "1." for simulation
//       m_globalDerivatives[shiftIndex] = -tangens;
      
//       float deltaX = parameters->GetParameter(shiftIndex);
//       float x0 = track->x0();
//       float lambda_x = track->slopeX();
//       m_globalDerivatives[rotIndex] = -deltaX - x0 - k*lambda_x + fx;

//       if (m_nLocal == 4) {
//         m_localDerivatives[0] = -tangens;
//         m_localDerivatives[1] = 1.;
//         m_localDerivatives[2] = -k*tangens;
//         m_localDerivatives[3] = k;
//       }
//       else if (m_nLocal == 2) {
//         m_localDerivatives[0] = 1.;
//         m_localDerivatives[1] = k;
//       }
//     }
//     else {
//       y = fx;
//       sigma = sqrt(V2(0,0));

//       // hardcoded for testbeam now, change "1." to -cotangens for simulation
//       m_globalDerivatives[shiftIndex] = 1.;

//       float deltaY = parameters->GetParameter(shiftIndex);
//       float y0 = track->y0();
//       float lambda_y = track->slopeY();
//       m_globalDerivatives[rotIndex] = -deltaY - y0 - k*lambda_y + fy;
       
//       if (m_nLocal == 4) {
//         m_localDerivatives[0] = 1.;
//         m_localDerivatives[1] = -cotangens;
//         m_localDerivatives[2] = k;
//         m_localDerivatives[3] = -k*cotangens;
//       }
//       else if (m_nLocal == 2) {
//         m_localDerivatives[0] = 1.;
//         m_localDerivatives[1] = k;
//       }
//     }

//     unsigned int hybrid = hit.hybrid();
//     unsigned int sipm = hit.sipm();
//     EQULOC(m_globalDerivatives, m_localDerivatives, y, sigma, hybrid, sipm);
//   }
// }

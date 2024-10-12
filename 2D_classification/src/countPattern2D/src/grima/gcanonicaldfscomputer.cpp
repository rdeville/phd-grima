/***************************************************************************
 *   Copyright (C) 2005 by Siegfried Nijssen                               *
 *   snijssen[at]informatik.uni-freiburg.de                                *
 * ----------------------------------------------------------------------- *
 *   Copyright (C) 2010,2011 by Adriana Prado and Baptiste Jeudy           *
 *   baptiste.jeudy[at]univ-st-etienne fr                                  *
 * ----------------------------------------------------------------------- *
 *   Copyright (C) 2014 by Romain Deville                                  *
 *   romain.deville[at]insa-lyon.fr                                        *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                         *
 ***************************************************************************/

//================================ INCLUDE ===================================//
// Include library class
#include <algorithm>
// Include project class
#include "gcanonicaldfscomputer.hpp"
#include "ggraph.hpp"

//=============================== NAMESPACE ==================================//
//============================ STRUCT & TYPEDEF ==============================//
//=============================== VARIABLES ==================================//
//================================ METHODS ===================================//
//================================= CLASS ====================================//

//---- PUBLIC --------------------------------------------------------------//
// Public CONSTANTS ________________________________________________________//
// Public Constructor/Desctructor __________________________________________//
GCanonicalDFSComputer::GCanonicalDFSComputer( GPattern *pPat ):
  pPattern( pPat ),
  v_CodeIndex( pPat->pGraph->v_Nodes.size() , GNONODEID ),
  v_GraphIndex( pPat->pGraph->v_Nodes.size() , GNONODEID )
{
  // Default constructor
  // Nothing to do here
}
// End of GCanonicalDFSComputer::GCanonicalDFSComputer( GPattern *pPat ):

GCanonicalDFSComputer::~GCanonicalDFSComputer()
{
  // Default Desctructor
  // Nothing to do here
}
// End of GCanonicalDFSComputer::~GCanonicalDFSComputer()

// Accessor ________________________________________________________________//
// Mutator _________________________________________________________________//
// Public Methods __________________________________________________________//
bool GCanonicalDFSComputer::isCanonincal()
{
  /*
   * @brief isCanonincal
   * Function that check if code of Pattern is canonical in three step.
   * Step One   : If the pattern is only one edge, then it's canonical
   * Step Two   : Check in the full DFS code of the pattern if there's no clue
   *              to directly know that pattern is not canonical.
   * Step Three : If step one and step two passed, then begin a recursion to
   *              construct all possible DFS Code of the pattern to find if
   *              there's a DFS Code greater than the current one.
   * @return TRUE if pattern is canonical, FALSE else.
   */

  // Initialize variables
  nbNodes     = 1 ;
  position    = 0;
  stop        = false;

  // === First Step ============================================================
  if ( pPattern->v_Tokens.size() == 1 )
  {
    // First iteration
    // If Pattern is only one edge, then return true
    return true;
  }


  // === Second Step ===========================================================
  // Instanciate variables for the second step
  GEdge firstEdge;
  GEdge tmpEdge;
  // Node id of the last node that wears an extension from 0
  vector<GCanonicalToken> v_firstEdges;
  firstEdge.nodeI = pPattern->v_Tokens[0].nodeLabelFrom;
  firstEdge.nodeJ = pPattern->v_Tokens[0].nodeLabelDest;
  for ( uint i = 0; i < pPattern->v_Tokens.size() ; i++ )
  {

    // For each code line
    tmpEdge.nodeI = pPattern->v_Tokens[i].nodeLabelFrom;
    tmpEdge.nodeJ = pPattern->v_Tokens[i].nodeLabelDest;
    if ( ( tmpEdge.nodeI > firstEdge.nodeI || tmpEdge.nodeJ > firstEdge.nodeI )
         || ( tmpEdge.nodeI >= firstEdge.nodeI && tmpEdge.nodeJ > firstEdge.nodeJ )
         || ( tmpEdge.nodeJ >= firstEdge.nodeI && tmpEdge.nodeI > firstEdge.nodeJ ) )
    {
      /* If there is a edge greater than the first one
       * i.e nodeLabel of this edge are greater than the first
       */
      return false;
    }
    else if ( ( ( tmpEdge.nodeI == firstEdge.nodeI
                  && tmpEdge.nodeJ == firstEdge.nodeJ )
                || ( tmpEdge.nodeJ == firstEdge.nodeI
                     && tmpEdge.nodeI == firstEdge.nodeJ ) ) )
    {
      /* If the tmpEdge nodes label are the same than the for first one of the
       * pattern
       */

      if ( pPattern->v_Tokens.at(0).angle == 0 )
      {
        // Make a list of possible first edge
        GCanonicalToken tmpToken;
        tmpToken.codeToken.direction     = gForward;
        tmpToken.codeToken.nodeFrom      = 0;
        tmpToken.codeToken.nodeDest      = 1;
        tmpToken.codeToken.angle         = 0;
        tmpToken.codeToken.nodeLabelFrom = pPattern->v_Tokens[i].nodeLabelFrom;
        tmpToken.codeToken.edgeLabel     = pPattern->v_Tokens[i].edgeLabel;
        tmpToken.codeToken.nodeLabelDest = pPattern->v_Tokens[i].nodeLabelDest;
        if ( firstEdge.nodeI == firstEdge.nodeJ )
        {
          tmpToken.nodeLargeI = pPattern->v_Tokens[i].nodeFrom;
          tmpToken.nodeLargeJ = pPattern->v_Tokens[i].nodeDest;
          v_firstEdges.push_back( tmpToken );
          tmpToken.nodeLargeI = pPattern->v_Tokens[i].nodeDest;
          tmpToken.nodeLargeJ = pPattern->v_Tokens[i].nodeFrom;
          v_firstEdges.push_back( tmpToken );
        }
        else if ( tmpEdge.nodeI == firstEdge.nodeI )
        {
          tmpToken.nodeLargeI = pPattern->v_Tokens[i].nodeFrom;
          tmpToken.nodeLargeJ = pPattern->v_Tokens[i].nodeDest;
          v_firstEdges.push_back( tmpToken );
        }
        else if ( tmpEdge.nodeJ == firstEdge.nodeI )
        {
          tmpToken.nodeLargeI = pPattern->v_Tokens[i].nodeDest;
          tmpToken.nodeLargeJ = pPattern->v_Tokens[i].nodeFrom;
          v_firstEdges.push_back( tmpToken );
        }
      }
      else if ( pPattern->v_Tokens.at(0).angle < 0 )
      {
        // Make a list of possible first edge
        GCanonicalToken tmpToken;
        tmpToken.codeToken.direction     = gForward;
        tmpToken.codeToken.nodeFrom      = 0;
        tmpToken.codeToken.nodeDest      = 1;
        tmpToken.codeToken.angle         = 0;
        tmpToken.codeToken.nodeLabelFrom = pPattern->v_Tokens[i].nodeLabelFrom;
        tmpToken.codeToken.edgeLabel     = pPattern->v_Tokens[i].edgeLabel;
        tmpToken.codeToken.nodeLabelDest = pPattern->v_Tokens[i].nodeLabelDest;
        if ( firstEdge.nodeI == firstEdge.nodeJ )
        {
          tmpToken.nodeLargeI = pPattern->v_Tokens[i].nodeFrom;
          tmpToken.nodeLargeJ = pPattern->v_Tokens[i].nodeDest;
          v_firstEdges.push_back( tmpToken );
          tmpToken.nodeLargeI = pPattern->v_Tokens[i].nodeDest;
          tmpToken.nodeLargeJ = pPattern->v_Tokens[i].nodeFrom;
          v_firstEdges.push_back( tmpToken );
        }
        else if ( tmpEdge.nodeI == firstEdge.nodeI )
        {
          tmpToken.nodeLargeI = pPattern->v_Tokens[i].nodeFrom;
          tmpToken.nodeLargeJ = pPattern->v_Tokens[i].nodeDest;
          v_firstEdges.push_back( tmpToken );
        }
        else if ( tmpEdge.nodeJ == firstEdge.nodeI )
        {
          tmpToken.nodeLargeI = pPattern->v_Tokens[i].nodeDest;
          tmpToken.nodeLargeJ = pPattern->v_Tokens[i].nodeFrom;
          v_firstEdges.push_back( tmpToken );
        }
      }
    }
  }

  for ( GNodeID nodeI = 0 ; nodeI <  v_firstEdges.size() ; nodeI++ )
  {
    // For each node of the pattern, build the code starting from node nodeI
    v_NewCode.push_back( v_firstEdges[nodeI] );
    recurse( GNOANGLE, v_firstEdges[nodeI].nodeLargeI, v_firstEdges[nodeI].nodeLargeJ);
    v_NewCode.erase( v_NewCode.begin(), v_NewCode.end() );
    if ( stop )
    {
      return false;
    }
  }

  /* ==============================================
   * TAG : NODE_OCCURENCY_FILTER
   * ============================================== */
  //  {
  //    map<int, int> m_NodeOcc;
  //    pair<int, int> tmpNodeOcc;
  //    m_NodeOcc.insert( tmpNodeOcc );
  //    for ( uint i = 0 ; i < pPattern->v_Tokens.size(); i++ )
  //    {
  //      if ( pPattern->v_Tokens[i].direction == GForward )
  //      {
  //        map<int, int>::iterator it = m_NodeOcc.find( pPattern->v_Tokens[i].nodeLabelDest);
  //        if ( it != m_NodeOcc.end() )
  //        {
  //          it->second++;
  //          if ( it->second >= 10 )
  //          {
  //            cerr << "WARNING : Following pattern have more that 10 times same node" << endl
  //                 << pPattern->v_Tokens << endl;
  //            return false;
  //          }
  //        }
  //        else
  //        {
  //          tmpNodeOcc.first  = pPattern->v_Tokens[i].nodeLabelDest;
  //          tmpNodeOcc.second = 1;
  //          m_NodeOcc.insert( tmpNodeOcc );
  //        }
  //      }
  //    }
  //  }
  return true;
}
// End of GCanonicalDFSComputer::isCanonincal()

//---- PROTECTED  ----------------------------------------------------------//
// Protected CONSTANTS _____________________________________________________//
// Protected Methods _______________________________________________________//

//---- PRIVATE -------------------------------------------------------------//
// Private CONSTANTS _______________________________________________________//
// Private Methods _________________________________________________________//
void GCanonicalDFSComputer::recurse( GEdgeAngle prevAngle,
                                     GNodeID nodeLargeI,
                                     GNodeID nodeLargeJ )
{
  /*
   * @brief recurse
   * Methods that construct all possible extension of the current new pattern
   * and store them in v_StackCode.
   * If prevAngle==HGNOANGLE,
   *  construct all extenstion from nodeLargeI and nodeLargeJ
   * Else
   *  construct only all exteion from nodeLargeJ
   * Once done, it will sort these extenstion and call subRecurse() to test it.
   * @param prevAngle  : The orientation of the previous edge
   * @param nodeLargeI  : The node from of the last construct edge
   * @param nodeLargeJ  : The node dest of the last construct edge
   */

  // Initialize variables
  uint v_NewCodeSize = v_StackCode.size();
  v_CodeIndex[nodeLargeJ] = nbNodes ;
  v_GraphIndex[nbNodes]   = nodeLargeJ;

  if ( prevAngle == GNOANGLE )
  {
    // If no previous angle is specified, meaning it's the first récursion
    // Map node 0
    v_CodeIndex[nodeLargeI] = 0 ;
    v_GraphIndex[0]         = nodeLargeI;
    for ( uint i = 0 ; i < pPattern->pGraph->v_Nodes[nodeLargeI].v_Edges.size() ; i++)
    {
      // For each existing edge from nodeLargeJ, construct possible extension
      GGraphEdge edge = pPattern->pGraph->v_Nodes[nodeLargeI].v_Edges[i];
      if ( edge.destNodeId != GNONODEID && edge.destNodeId != nodeLargeJ )
      {
        GCanonicalToken token;
        token.nodeLargeI = nodeLargeI;
        token.nodeLargeJ = edge.destNodeId;
        token.codeToken.nodeFrom = v_CodeIndex[nodeLargeI] ;
        int prevPos = pPattern->pGraph->getEdgePositionFrom(  nodeLargeJ, nodeLargeI );
        token.codeToken.angle = getTokenAngleValue( prevPos,
                                                    token.nodeLargeI,
                                                    token.nodeLargeJ,
                                                    pPattern );
        token.codeToken.nodeLabelFrom = pPattern->pGraph->v_Nodes[nodeLargeI].label;
        token.codeToken.nodeLabelDest = pPattern->pGraph->v_Nodes[edge.destNodeId].label;
        token.codeToken.edgeLabel     = edge.label;
        if ( v_CodeIndex[edge.destNodeId] == GNONODEID )
        {
          // If destination node not already met
          token.codeToken.direction = gForward;
          token.codeToken.nodeDest  = GNONODEID;
          v_StackCode.push_back( token );
        }
        else
        {
          token.codeToken.direction = gBackward;
          token.codeToken.nodeDest  = GNONODEID;
          v_StackCode.push_back( token );
        }
      }
    }
  }
  for ( uint i = 0 ; i < pPattern->pGraph->v_Nodes[nodeLargeJ].v_Edges.size() ; i++)
  {
    // For each existing edge from nodeLargeJ, construct possible extension
    GGraphEdge edge = pPattern->pGraph->v_Nodes[nodeLargeJ].v_Edges[i];
    if ( edge.destNodeId != GNONODEID && edge.destNodeId != nodeLargeI )
    {
      GCanonicalToken token;
      token.nodeLargeI = nodeLargeJ;
      token.nodeLargeJ = edge.destNodeId;
      token.codeToken.nodeFrom = v_CodeIndex[nodeLargeJ] ;
      int prevPos = pPattern->pGraph->getEdgePositionFrom(  nodeLargeI, nodeLargeJ );
      token.codeToken.angle = getTokenAngleValue( prevPos,
                                                  token.nodeLargeI,
                                                  token.nodeLargeJ,
                                                  pPattern );
      token.codeToken.nodeLabelFrom = pPattern->pGraph->v_Nodes[nodeLargeJ].label;
      token.codeToken.nodeLabelDest = pPattern->pGraph->v_Nodes[edge.destNodeId].label;
      token.codeToken.edgeLabel     = edge.label;
      if ( v_CodeIndex[edge.destNodeId] == GNONODEID )
      {
        // If destination node not already met
        token.codeToken.direction = gForward;
        token.codeToken.nodeDest  = GNONODEID;
        v_StackCode.push_back( token );
      }
      else
      {
        token.codeToken.direction = gBackward;
        token.codeToken.nodeDest  = GNONODEID;
        v_StackCode.push_back( token );
      }
    }
  }
  if ( v_NewCode.size() < pPattern->v_Tokens.size() && v_StackCode.empty() )
  {
    for ( uint i = 0 ; i < pPattern->pGraph->v_Nodes[ v_GraphIndex[0] ].v_Edges.size() ; i++)
    {
      // For each existing  edge from node 0, construct possible extension
      GGraphEdge edge = pPattern->pGraph->v_Nodes[ v_GraphIndex[0] ].v_Edges[i];
      if ( edge.destNodeId != GNONODEID && edge.destNodeId !=  v_GraphIndex[1]  )
      {
        GCanonicalToken token;
        token.nodeLargeI = v_GraphIndex[0] ;
        token.nodeLargeJ = edge.destNodeId;
        token.codeToken.direction = gForward;
        token.codeToken.nodeFrom  = v_CodeIndex[ v_GraphIndex[0] ] ;
        token.codeToken.nodeDest  = GNONODEID;
        int prevPos = pPattern->pGraph->getEdgePositionFrom( v_GraphIndex[1], v_GraphIndex[0] );
        token.codeToken.angle = getTokenAngleValue( prevPos,
                                                    token.nodeLargeI,
                                                    token.nodeLargeJ,
                                                    pPattern );
        token.codeToken.nodeLabelFrom = pPattern->pGraph->v_Nodes[ v_GraphIndex[0] ].label;
        token.codeToken.edgeLabel     = edge.label;
        token.codeToken.nodeLabelDest = pPattern->pGraph->v_Nodes[edge.destNodeId].label;
        v_StackCode.push_back( token );
      }
    }
  }


  if ( v_NewCodeSize != v_StackCode.size() )
  {
    // If we added extension to the existing ones, sort these new extensions
    sort( v_StackCode.begin() + v_NewCodeSize, v_StackCode.end(), HGCanonicalDFSTokenLt () );
  }

  ++nbNodes;
  subRecurse();
  --nbNodes;

  // Undo mapping of the last node
  v_CodeIndex[nodeLargeJ] = GNONODEID;
  if ( prevAngle == GNOANGLE )
  {
    // If first iteration, undo mapping of the node 0
    v_CodeIndex[nodeLargeI] = GNONODEID;
  }
  v_StackCode.resize( v_NewCodeSize );
}
// End of GCanonicalDFSComputer::recurse( GEdgeAngle prevAngle,
//                                            GNodeID nodeLargeI,
//                                            GNodeID nodeLargeJ )

void GCanonicalDFSComputer::subRecurse( )
{
  /*
   * @brief subRecurse
   * Methods that will add the greatest extension previously construct from
   * recurse() to the new DFS Code and compare it to the equivalent line code of
   * the pattern to test.
   * If the pattern DFS code is less than the new one
   *  the pattern DFS Code is not canonical, exit the test
   * Else if the pattern DFS code is greater than the new one
   *  Exit subRecurse() to test another extension
   * Call recurse to extends the current new pattern from the last node or all
   * the previous one
   */

  // Instanciate and update variable
  GCanonicalToken token;
  GCanonicalToken saveToken;
  int cmp;
  int patternSize = pPattern->v_Tokens.size();
  ++position;

  if ( v_StackCode.empty() || position == patternSize )
  {
    // If there's no more extension
    goto cleanup;
  }

  // Set the last extension of v_StackCode, i.e. the greatest one

  if ( v_StackCode.back().codeToken.direction == gForward)
  {
    token = v_StackCode.back();
    token.codeToken.nodeDest = nbNodes;
    v_NewCode.push_back(v_StackCode.back());
    cmp = cmpGToken ( pPattern->v_Tokens[position], token.codeToken );
    token.codeToken.nodeDest = GNONODEID;
  }
  else
  {
    vector<GCanonicalToken> v_TempToken;
    while ( v_StackCode.back().codeToken.direction == gBackward )
    {
      token = v_StackCode.back();
      token.codeToken.nodeDest = v_CodeIndex[token.nodeLargeJ];
      v_TempToken.push_back( token );
      v_StackCode.pop_back();
    }
    sort( v_TempToken.begin(), v_TempToken.end(), HGCanonicalDFSTokenLt () );
    token = v_TempToken.back();
    cmp = cmpGToken ( pPattern->v_Tokens[position], token.codeToken );
    for ( uint i = 0; i < v_TempToken.size(); i++ )
    {
      v_TempToken.at(i).codeToken.nodeDest = GNONODEID;
      v_StackCode.push_back( v_TempToken.at(i) );
    }
    v_NewCode.push_back( v_TempToken.back() );
  }

  // Compare new extension
  if ( cmp < 0 )
  {
    // If pPattern DFS code < current DFS tested code.
    // i.e. pPattern DFS code not canonical.
    stop = true;
    goto cleanup;
  }
  else if ( cmp > 0 )
  {
    // Else pPattern code > current DFS tested code.
    // i.e. current DFS tested code not canonical.
    goto cleanup;
  }
  // End if, mean pPattern code and current DFS code are the same.

  saveToken = v_StackCode.back();
  v_StackCode.pop_back();
  if ( saveToken.codeToken.direction == gForward )
  {
    // IF last added edge was forward, try to extends from the last node added
    recurse( saveToken.codeToken.angle, saveToken.nodeLargeI, saveToken.nodeLargeJ );
  }
  else
  {
    // If last added edge was backward, try to extends from previous last node added
    // remove equivalent edge which is forward and but it into buffer.
    for ( uint i = 0; i < v_StackCode.size(); i++ )
    {
      if ( v_StackCode.at(i).nodeLargeI == saveToken.nodeLargeJ
           && v_StackCode.at(i).nodeLargeJ == saveToken.nodeLargeI )
      {
        v_Buffer.push_back( v_StackCode.at( i ) );
        v_StackCode.erase( v_StackCode.begin() + i );
      }
    }
    subRecurse();
  }
  for ( uint i = 0; i < v_Buffer.size(); i++ )
  {
    if ( v_Buffer.at(i).nodeLargeI == saveToken.nodeLargeJ
         && v_Buffer.at(i).nodeLargeJ == saveToken.nodeLargeI )
    {
      v_StackCode.push_back( v_Buffer.at(i) );
      v_Buffer.erase( v_Buffer.begin() + i );
    }
  }
  v_StackCode.push_back( saveToken );
  v_NewCode.pop_back();


cleanup:
  --position;
}
// End of GCanonicalDFSComputer::subRecurse



GEdgeAngle GCanonicalDFSComputer::getTokenAngleValue( uint    prevEdgeFrom,
                                                      GNodeID nodeFrom,
                                                      GNodeID nodeDest,
                                                      GPattern *pPattern)
{
  /*
   * TODO : RD
   * Write Desc
   */
  if ( prevEdgeFrom < 4 )
  {
    // If previous edge is spatial
    uint currEdgePos = pPattern->pGraph->getEdgePositionFrom( nodeFrom, nodeDest );
    if ( currEdgePos >= 4 )
    {
      if ( currEdgePos == 4 )
        return -1;
      else
        return -2;
      //return currEdgePos;
    }
    else if ( prevEdgeFrom == 0 )
    {
      if ( currEdgePos == 0 )
        return 2;
      else if ( currEdgePos == 1 )
        return 1;
      else if ( currEdgePos == 2 )
        return 0;
      else if ( currEdgePos == 3 )
        return 3;
    }
    else if ( prevEdgeFrom == 1 )
    {
      if ( currEdgePos == 0 )
        return 3;
      else if ( currEdgePos == 1 )
        return 2;
      else if ( currEdgePos == 2 )
        return 1;
      else if ( currEdgePos == 3 )
        return 0;
    }
    else if ( prevEdgeFrom == 2 )
    {
      if ( currEdgePos == 0 )
        return 0;
      else if ( currEdgePos == 1 )
        return 3;
      else if ( currEdgePos == 2 )
        return 2;
      else if ( currEdgePos == 3 )
        return 1;
    }
    else if ( prevEdgeFrom == 3 )
    {
      if ( currEdgePos == 0 )
        return 1;
      else if ( currEdgePos == 1 )
        return 0;
      else if ( currEdgePos == 2 )
        return 3;
      else if ( currEdgePos == 3 )
        return 2;
    }
  }
  else if ( pPattern->v_Tokens.at(0).angle < 0 )
  {
    uint currEdgePos = pPattern->pGraph->getEdgePositionFrom( nodeFrom, nodeDest );
    if ( currEdgePos ==  4 )
      return -1;
    else if ( currEdgePos ==  5 )
      return -2;
    else
      return 0;
  }
  else
  {
    // Previous edge is temporal, need to find first spatial edge that arrive on
    // nodeFrom
    uint iToken;
    GNodeID nodeLargeFrom ;
    GNodeID nodeLargeDest ;

    if ( nodeFrom == 0 )
    {
      // Previous edge was temporal starting from 0, then take edge (0,1)
      nodeLargeFrom = 1;
      nodeLargeDest = 0;
    }
    else
    {
      iToken = findNotTemporalToken( nodeFrom );

      if ( pPattern->v_Tokens.at(iToken).nodeFrom == 0 && iToken != 0 )
      {
        // Previous edge was temporal starting from 0, then take edge (0,1)
        nodeLargeFrom = 1;
        nodeLargeDest = 0;
      }
      else
      {
        // Previous edge was temporal not starting from 0
        nodeLargeFrom = pPattern->v_Tokens.at(iToken).nodeFrom;
        nodeLargeDest = pPattern->v_Tokens.at(iToken).nodeDest;
      }
    }
    uint currSpatEdgePos = pPattern->pGraph->getEdgePositionFrom( nodeFrom, nodeDest );
    uint prevSpatEdgePos = pPattern->pGraph->getEdgePositionFrom( nodeLargeFrom, nodeLargeDest );

    uint angleValue;

    if ( currSpatEdgePos >= 4 )
    {
      if ( currSpatEdgePos == 4 )
        angleValue = -1;
      else
        angleValue = -2;
      //angleValue = currSpatEdgePos;
    //return currSpatEdgePos;
    }
    if ( currSpatEdgePos >= 4 )
    {
      if ( currSpatEdgePos == 4 )
        angleValue = -1;
      else
        angleValue = -2;
    }
    else if ( prevSpatEdgePos == 0 )
    {
      if ( currSpatEdgePos == 0 )
        angleValue = 2;
      else if ( currSpatEdgePos == 1 )
        angleValue = 1;
      else if ( currSpatEdgePos == 2 )
        angleValue = 0;
      else if ( currSpatEdgePos == 3 )
        angleValue = 3;
    }
    else if ( prevSpatEdgePos == 1 )
    {
      if ( currSpatEdgePos == 0 )
        angleValue = 3;
      else if ( currSpatEdgePos == 1 )
        angleValue = 2;
      else if ( currSpatEdgePos == 2 )
        angleValue = 1;
      else if ( currSpatEdgePos == 3 )
        angleValue = 0;
    }
    else if ( prevSpatEdgePos == 2 )
    {
      if ( currSpatEdgePos == 0 )
        angleValue = 0;
      else if ( currSpatEdgePos == 1 )
        angleValue = 3;
      else if ( currSpatEdgePos == 2 )
        angleValue = 2;
      else if ( currSpatEdgePos == 3 )
        angleValue = 1;
    }
    else if ( prevSpatEdgePos == 3 )
    {
      if ( currSpatEdgePos == 0 )
        angleValue = 1;
      else if ( currSpatEdgePos == 1 )
        angleValue = 0;
      else if ( currSpatEdgePos == 2 )
        angleValue = 3;
      else if ( currSpatEdgePos == 3 )
        angleValue = 2;
    }
    return angleValue;
  }
  return GNOANGLE;
}
// End of GSubgraphIso::getAngleValue( uint    prevEdgeFrom,
//                                     GNodeID nodeFrom,
//                                     GNodeID nodeDest )

int GCanonicalDFSComputer::findNotTemporalToken( GNodeID patNodeFrom )
{
  uint iToken = 0;
  while ( pPattern->v_Tokens.at(iToken).nodeDest != patNodeFrom
          || pPattern->v_Tokens.at(iToken).direction != gForward )
    iToken++;

  if ( pPattern->v_Tokens.at(iToken).angle < 0 )
  {
    if ( pPattern->v_Tokens.at(iToken).nodeFrom == 0 )
      return iToken;
    else
      return findNotTemporalToken( pPattern->v_Tokens.at(iToken).nodeFrom );
  }
  else
    return iToken;
}

//============================== OPERATOR OVERLOAD  ==========================//

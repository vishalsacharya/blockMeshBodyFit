/*---------------------------------------------------------------------------*\
=========                 |
\\      /  F ield         | Unsupported Contributions for OpenFOAM
 \\    /   O peration     |
  \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
   \\/     M anipulation  |
-------------------------------------------------------------------------------
2013-06-01 Karl-Johan Nogenmyr 

Added:
        label vtxLabelNoBoundary(...)
        vector getPoint(...)
        void getProjection(...)

Modified calls to createPoints(...) and points(...)

License
    This file is a derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::block

Description
    Creates a single block of cells from point coordinates, numbers of
    cells in each direction and an expansion ratio.

Note
    The vertices and cells for filling the block are demand-driven.

SourceFiles
    block.C
    blockCreate.C

\*---------------------------------------------------------------------------*/

#ifndef block_H
#define block_H

#include "pointField.H"
#include "labelList.H"

#include "blockDescriptor.H"
#include "triSurfaceMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class Istream;
class Ostream;

// Forward declaration of friend functions and operators
class block;
Ostream& operator<<(Ostream&, const block&);

/*---------------------------------------------------------------------------*\
                           Class block Declaration
\*---------------------------------------------------------------------------*/

class block
:
    public blockDescriptor
{
    // Private data

        //- List of vertices
        mutable pointField vertices_;

        //- List of cells
        mutable labelListList cells_;

        //- Boundary patches
        mutable labelListListList boundaryPatches_;


    // Private Member Functions

        //- Vertex label offset for a particular i,j,k position on an interior mesh
        label vtxLabelNoBoundary(label i, label j, label k) const;

        //- Basically blockMesh's original point position code
        vector getPoint(label i, label j, label k, bool flat) const;

        //- Give projected point
        void getProjection
        (
            point& p,
            vector& normal,
            const triSurfaceMesh& snapSurface,
            const scalar searchLength
        ) const;
        
        //- Creates vertices for cells filling the block
        void createPoints
        (
            const PtrList<triSurfaceMesh>& snapSurfaces,
            const labelList& snapList,
            const faceList& allBlocksFaces,  // all blocks' faces
            const vectorField& pointNormals,
            const scalar searchLength
        ) const;

        //- Creates cells for filling the block
        void createCells() const;

        //- Creates boundary patch faces for the block
        void createBoundary() const;

        //- Disallow default bitwise copy construct
        block(const block&);

        //- Disallow default bitwise assignment
        void operator=(const block&);

public:

    // Constructors

        //- Construct from components with Istream
        block
        (
            const pointField& blockPointField,
            const curvedEdgeList&,
            Istream&
        );

        //- Construct from a block definition
        block(const blockDescriptor&);

        //- Clone
        autoPtr<block> clone() const
        {
            notImplemented("block::clone()");
            return autoPtr<block>(NULL);
        }


    //- Destructor
    ~block();


    // Member Functions

        // Access

            //- Return the block definition
            inline const blockDescriptor& blockDef() const
            {
                return *this;
            }

            //- Vertex label offset for a particular i,j,k position
            label vtxLabel(label i, label j, label k) const;

            //- Return the points for filling the block
            const pointField& points
            (
                const PtrList<triSurfaceMesh>& snapSurfaces,
                const labelList& snapList,
                const faceList& allBlocksFaces,
                const vectorField& pointNormals,
                const scalar searchLength
            ) const;

            //- Return the cells for filling the block
            const labelListList& cells() const;

            //- Return the boundary patch faces for the block
            const labelListListList& boundaryPatches() const;


        // Edit

            //- Clear geometry (internal points, cells, boundaryPatches)
            void clearGeom();


    // Ostream Operator

        friend Ostream& operator<<(Ostream&, const block&);
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

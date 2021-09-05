/*! ****************************************************************************
\file                Transforms.h
\author              Chyler Morrison

\par  DigiPen login: morrison.c

\par        Project: Radiant (Volunteer)

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*******************************************************************************/

#ifndef __TRANSFORM_H
#define __TRANSFORM_H

// Include Files                ////////////////////////////////////////////////

#include "../component/component.h"
#include "Vector2D.h"
#include "Matrix2D.h"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Forward Namespace References ////////////////////////////////////////////////

// Public Namespace Enums       ////////////////////////////////////////////////

// Public Namespace Objects     ////////////////////////////////////////////////

/*! ****************************************************************************
\brief
  Component to allow spatial calculations of objects.
*******************************************************************************/
class Transform : public Component<Transform>
{
private:

  // Members              ///////////////////////

    /// Position
  Vector2D m_Translation;
    /// Scale
  Vector2D m_Scale;
    /// Angle (assumed radians)
  float m_Angle;

  /// Old Position
  Vector2D oldTranslation;

  /// old scale
  Vector2D oldScale;

  /// old angle (radians)
  float oldAngle;

  /// old transform
  Matrix2D oldMatrix;

    /// Matrix containing the calculated transformation
  Matrix2D m_Matrix;
    /// Bool for calculating the transformation when its needed (optimization)
  bool m_MatrixDirty;

public:

  // Members              ///////////////////////

    /// Static c-string for the name of this component
  COMPONENT_NAME(Transform);

  // Con-/De- structors   ///////////////////////

  /*! **************************************************************************
  \brief
    Default constructor. Default values:
    Scale - 1x1
    Rotation - 0
    Position - 0,0
  *****************************************************************************/
  Transform();

  Transform(Transform const &) = default; ///< Default copy constructor
  Transform(Transform &&) = default;      ///< Default move constructor

  ~Transform() = default;                 ///< Default destructor

  // Operators            ///////////////////////

  Transform & operator=(Transform const &) = default; ///< Default copy assignment
  Transform & operator=(Transform &&) = default;      ///< Default move assignment

  /*! ************************************************************************
  \brief
    Returns the transformation matrix. If the transform's parameters were
    updated between now and the previous call, the matrix will be recalculated.

  \return
    The transformation matrix
  *****************************************************************************/
  operator Matrix2D();

  // Accossors/Mutators   ///////////////////////

    /// Returns the translation (position) vector
  Vector2D const & Translation() const;
  Vector2D const& OldTranslation() const {
	  return oldTranslation;
  };
    /// Returns the rotation angle (radians)
  float Rotation() const;
  float OldRotation() const {
	  return oldAngle;
  };
    /// Retuns the scale
  Vector2D const & Scale() const;
  Vector2D const& OldScale() const { return oldScale; };
    /// Sets the translation
  void Translation(Vector2D const &);
  void OldTranslation(Vector2D const& t) { oldTranslation = t; };
    /// Sets the rotation angle (radians)
  void Rotation(float);
  void OldRotation(float r) { oldAngle; };
    /// Sets the scale
  void Scale(Vector2D const &);
  void OldScale(Vector2D const& s) { oldScale = s; };

  bool isDirty() const { return m_MatrixDirty; };

  // Functions            ///////////////////////

  virtual void Update(float dt) {}; ///< Defining inherited virtual function
  virtual void Draw() const {};     ///< Defining inherited virtual function

private:

  // Functions                  ///////////////////////

}; // class Transform

// Public Namespace Functions   ////////////////////////////////////////////////

#endif // __TRANSFORM_H

#include "MeleeWeapon.h"
#include "Math\Matrix44.h"
#include "Math\Vector3.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined (_DEBUG)
#include "Memory/MemLeaks.h"
#endif

CMeleeWeapon::CMeleeWeapon()
{

}

CMeleeWeapon::~CMeleeWeapon()
{

}

void CMeleeWeapon::Update(float _ElapsedTime)
{
	Vect3f l_TranslationLeft		= v3fZERO;
	Vect3f l_TranslationRight		= v3fZERO;
	Vect4f l_RotationLeft			= v3fZERO;
	Vect4f l_RotationRight			= v3fZERO;
	Mat44f l_TransformMatrix		= m44fIDENTITY;
	Mat44f l_RotationMatrix			= m44fIDENTITY;
	Mat44f l_AnimatedModelTransform = m_pAnimatedModel->GetTransform();

	if( m_WeaponHandType == LEFT_HAND || m_WeaponHandType == BOTH_HAND )
	{
		m_pAnimatedModel->GetBonePosition(m_LeftHandName, l_TranslationLeft);
		m_pAnimatedModel->GetBoneRotation(m_LeftHandName, l_RotationLeft);

		l_TransformMatrix.Translate(l_TranslationLeft);
		l_RotationMatrix.SetFromQuaternion(l_RotationLeft);

		l_TransformMatrix = l_AnimatedModelTransform * l_TransformMatrix * l_RotationMatrix;

		m_pWeaponLeft->SetPosition( l_TransformMatrix.GetPos() );
		m_pWeaponLeft->SetYaw(  mathUtils::Rad2Deg(l_TransformMatrix.GetYaw()) );
		m_pWeaponLeft->SetPitch( mathUtils::Rad2Deg(l_TransformMatrix.GetPitch()) );
		m_pWeaponLeft->SetRoll( mathUtils::Rad2Deg(l_TransformMatrix.GetRoll()) );
	}

	if( m_WeaponHandType == RIGHT_HAND || m_WeaponHandType == BOTH_HAND )
	{
		l_TransformMatrix.SetIdentity();
		l_RotationMatrix.SetIdentity();

		m_pAnimatedModel->GetBonePosition(m_RightHandName, l_TranslationRight);
		m_pAnimatedModel->GetBoneRotation(m_RightHandName, l_RotationRight);

		l_TransformMatrix.Translate(l_TranslationRight);
		l_RotationMatrix.SetFromQuaternion(l_RotationRight);

		l_TransformMatrix = l_AnimatedModelTransform * l_TransformMatrix * l_RotationMatrix;

		m_pWeaponRight->SetPosition( l_TransformMatrix.GetPos() );
		m_pWeaponRight->SetYaw( mathUtils::Rad2Deg(l_TransformMatrix.GetYaw()) );
		m_pWeaponRight->SetPitch( mathUtils::Rad2Deg(l_TransformMatrix.GetPitch()) );
		m_pWeaponRight->SetRoll( mathUtils::Rad2Deg(l_TransformMatrix.GetRoll()) );
	}
}
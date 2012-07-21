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
	Mat44f l_TransformMatrix		= m44fIDENTITY;
	Mat44f l_AnimatedModelTransform = m_pAnimatedModel->GetTransform();

	if( m_WeaponHandType == LEFT_HAND || m_WeaponHandType == BOTH_HAND )
	{
		m_pAnimatedModel->GetBonePosition(m_LeftHandName, l_TranslationLeft);
		l_TransformMatrix.Translate(l_TranslationLeft);

		l_TransformMatrix = l_AnimatedModelTransform * l_TransformMatrix;

		m_pWeaponLeft->SetPosition( l_TransformMatrix.GetPos() );
		m_pWeaponLeft->SetYaw( l_TransformMatrix.GetYaw() );
		m_pWeaponLeft->SetPitch( l_TransformMatrix.GetPitch() );
		m_pWeaponLeft->SetRoll( l_TransformMatrix.GetRoll() );
	}

	if( m_WeaponHandType == RIGHT_HAND || m_WeaponHandType == BOTH_HAND )
	{
		l_TransformMatrix.SetIdentity();

		m_pAnimatedModel->GetBonePosition(m_RightHandName, l_TranslationRight);
		l_TransformMatrix.Translate(l_TranslationRight);

		l_TransformMatrix = l_AnimatedModelTransform * l_TransformMatrix;

		m_pWeaponRight->SetPosition( l_TransformMatrix.GetPos() );
		m_pWeaponRight->SetYaw( l_TransformMatrix.GetYaw() );
		m_pWeaponRight->SetPitch( l_TransformMatrix.GetPitch() );
		m_pWeaponRight->SetRoll( l_TransformMatrix.GetRoll() );
	}
}
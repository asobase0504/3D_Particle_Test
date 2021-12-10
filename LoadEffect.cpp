//**************************************************************************************************
//
// �G�t�F�N�g�̓Ǎ�����
// Auther�FYuda Kaito
//
//**************************************************************************************************
//************************************************************
// include
//************************************************************
#include "main.h"
#include "effect.h"
#include "LoadEffect.h"
#include <assert.h>

//************************************************************
// �}�N����`
//************************************************************

//************************************************************
// �v���g�^�C�v�錾
//************************************************************

//*******************************************************************************
// �ÓI�ϐ�
//*******************************************************************************

//-----------------------
// �t�@�C����
//-----------------------
static const char* pFilename[] =
{
	"data/FILE/TestParticle.txt",
	"data/FILE/Particle.txt",
};
// PFile �� TYPEEFFECT �̐����`�F�b�N
static_assert((sizeof(pFilename) / sizeof(pFilename[0])) == MAX_TYPE_EFFECT, "error �t�@�C���̐��Ɨ񋓌^�̐�����v���Ă܂���");

//********************************************************************************
// �e�N�X�`���̓ǂݍ��ݏ���
//********************************************************************************
Effect LoadTex(EFFECT_TEX type)
{
	return Effect();
}

//********************************************************************************
// �G�t�F�N�g�̓ǂݍ��ݏ���
//********************************************************************************
Effect LoadEffect(EFFECT_TYPE type)
{
	assert(type >= 0 && type < MAX_TYPE_EFFECT);

	FILE* pFile;
	Effect effect;

	ZeroMemory(&effect, sizeof(effect));

	// �t�@�C�����J��
	pFile = fopen(pFilename[type], "r");

	char read[255] = {};

	// �ǂݍ��ݏ���
	while (1)
	{
		fscanf(pFile, "%s", &read);

		if (strcmp(&read[0], "TEXTURE") == 0)
		{
			fscanf(pFile, "%s", &read);			// = �̏���
			fscanf(pFile, "%d", &effect.tex);	// �l������

		}
		else if (strcmp(&read[0], "ColRed") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.colR.fValue);	// �l������
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.colR.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.colR.initial.fMax);
					fscanf(pFile, "%f", &effect.colR.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.colR.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.colR.Add.fMax);
					fscanf(pFile, "%f", &effect.colR.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// �����t�^�I��
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "ColBlue") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.colB.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.colB.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.colB.initial.fMax);
					fscanf(pFile, "%f", &effect.colB.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.colB.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.colB.Add.fMax);
					fscanf(pFile, "%f", &effect.colB.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// �����t�^�I��
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "ColGreen") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.colG.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.colG.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.colG.initial.fMax);
					fscanf(pFile, "%f", &effect.colG.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.colG.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.colG.Add.fMax);
					fscanf(pFile, "%f", &effect.colG.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// �����t�^�I��
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "ColAlpha") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.colA.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.colA.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.colA.initial.fMax);
					fscanf(pFile, "%f", &effect.colA.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.colA.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.colA.Add.fMax);
					fscanf(pFile, "%f", &effect.colA.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// �����t�^�I��
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "Radius") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.randRadius.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.randRadius.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.randRadius.initial.fMax);
					fscanf(pFile, "%f", &effect.randRadius.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.randRadius.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.randRadius.Add.fMax);
					fscanf(pFile, "%f", &effect.randRadius.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// �����t�^�I��
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "Life") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%d", &effect.randLife.nValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.randLife.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%d", &effect.randLife.initial.nMax);
					fscanf(pFile, "%d", &effect.randLife.initial.nMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.randLife.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%d", &effect.randLife.Add.nMax);
					fscanf(pFile, "%d", &effect.randLife.Add.nMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// �����t�^�I��
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "Speed") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.randSpeed.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.randSpeed.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.randSpeed.initial.fMax);
					fscanf(pFile, "%f", &effect.randSpeed.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.randSpeed.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.randSpeed.Add.fMax);
					fscanf(pFile, "%f", &effect.randSpeed.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// �����t�^�I��
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "Aangle_X") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.shotAngleX.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.shotAngleX.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.shotAngleX.initial.fMax);
					fscanf(pFile, "%f", &effect.shotAngleX.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.shotAngleX.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.shotAngleX.Add.fMax);
					fscanf(pFile, "%f", &effect.shotAngleX.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// �����t�^�I��
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "Aangle_Y") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.shotAngleY.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.shotAngleZ.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.shotAngleY.initial.fMax);
					fscanf(pFile, "%f", &effect.shotAngleY.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.shotAngleY.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.shotAngleY.Add.fMax);
					fscanf(pFile, "%f", &effect.shotAngleY.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// �����t�^�I��
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "Aangle_Z") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.shotAngleZ.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.shotAngleZ.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.shotAngleZ.initial.fMax);
					fscanf(pFile, "%f", &effect.shotAngleZ.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.shotAngleZ.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.shotAngleZ.Add.fMax);
					fscanf(pFile, "%f", &effect.shotAngleZ.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// �����t�^�I��
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "gravity") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = �̏���
					fscanf(pFile, "%f", &effect.fGravity);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{

				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{

				}
				else if (strcmp(&read[0], "end") == 0)			// �����t�^�I��
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "FileEnd") == 0)
		{
			break;
		}
	}

	fclose(pFile);
	return effect;
}
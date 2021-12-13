#include "SaveEffect.h"

//********************************************************************************
// �G�t�F�N�g�̃Z�[�u����
//********************************************************************************
void SaveEffect(Effect* effect)
{
	FILE * pFile;

	//�t�@�C�����J��
	pFile = fopen("data/FILE/Particle2.txt", "w");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
	 // �f�[�^�̓ǂݍ���
	 // Red�J���[
		fprintf(pFile, "TEXTURE = %d\n\n", (int)effect->tex);

		// �Z�b�g����Pos�̂���
		fprintf(pFile, "posLag\n");
		fprintf(pFile, "nValue = %f %f %f\n", effect->posRand.x, effect->posRand.y, effect->posRand.z);
		fprintf(pFile, "end\n\n");

		fprintf(pFile, "ColRed\n");
		fprintf(pFile, "nValue = %f\n", effect->colR.fValue);
		if (effect->colR.initial.bIsRandom)
		{
			fprintf(pFile, "InitialRand = %f %f\n", effect->colR.initial.fMax, effect->colR.initial.fMin);
		}
		if (effect->colR.Add.bIsRandom)
		{
			fprintf(pFile, "AddRand = %f %f\n", effect->colR.Add.fMax, effect->colR.Add.fMin);
		}
		fprintf(pFile, "end\n\n");

		// Blue�J���[
		fprintf(pFile, "ColBlue\n");
		fprintf(pFile, "nValue = %f\n", effect->colB.fValue);
		if (effect->colB.initial.bIsRandom)
		{
			fprintf(pFile, "InitialRand = %f %f\n", effect->colB.initial.fMax, effect->colB.initial.fMin);
		}
		if (effect->colB.Add.bIsRandom)
		{
			fprintf(pFile, "AddRand = %f %f\n", effect->colB.Add.fMax, effect->colB.Add.fMin);
		}
		fprintf(pFile, "end\n\n");

		// Green�J���[
		fprintf(pFile, "ColGreen\n");
		fprintf(pFile, "nValue = %f\n", effect->colG.fValue);
		if (effect->colG.initial.bIsRandom)
		{
			fprintf(pFile, "InitialRand = %f %f\n", effect->colG.initial.fMax, effect->colG.initial.fMin);
		}
		if (effect->colG.Add.bIsRandom)
		{
			fprintf(pFile, "AddRand = %f %f\n", effect->colG.Add.fMax, effect->colG.Add.fMin);
		}
		fprintf(pFile, "end\n\n");

		// Alpha�J���[
		fprintf(pFile, "ColAlpha\n");
		fprintf(pFile, "nValue = %f\n", effect->colA.fValue);
		if (effect->colA.initial.bIsRandom)
		{
			fprintf(pFile, "InitialRand = %f %f\n", effect->colA.initial.fMax, effect->colA.initial.fMin);
		}
		if (effect->colA.Add.bIsRandom)
		{
			fprintf(pFile, "AddRand = %f %f\n", effect->colA.Add.fMax, effect->colA.Add.fMin);
		}
		fprintf(pFile, "end\n\n");

		// ��
		fprintf(pFile, "Width\n");
		fprintf(pFile, "nValue = %f\n", effect->width.fValue);
		if (effect->width.initial.bIsRandom)
		{
			fprintf(pFile, "InitialRand = %f %f\n", effect->width.initial.fMax, effect->width.initial.fMin);
		}
		if (effect->width.Add.bIsRandom)
		{
			fprintf(pFile, "AddRand = %f %f\n", effect->width.Add.fMax, effect->width.Add.fMin);
		}
		fprintf(pFile, "end\n\n");

		// ����
		fprintf(pFile, "Height\n");
		fprintf(pFile, "nValue = %f\n", effect->height.fValue);
		if (effect->height.initial.bIsRandom)
		{
			fprintf(pFile, "InitialRand = %f %f\n", effect->height.initial.fMax, effect->height.initial.fMin);
		}
		if (effect->height.Add.bIsRandom)
		{
			fprintf(pFile, "AddRand = %f %f\n", effect->height.Add.fMax, effect->height.Add.fMin);
		}
		fprintf(pFile, "end\n\n");

		// ����
		fprintf(pFile, "Life\n");
		fprintf(pFile, "nValue = %d\n", effect->randLife.nValue);
		if (effect->randLife.initial.bIsRandom)
		{
			fprintf(pFile, "InitialRand = %d %d\n", effect->randLife.initial.nMax, effect->randLife.initial.nMin);
		}
		if (effect->randLife.Add.bIsRandom)
		{
			fprintf(pFile, "AddRand = %d %d\n", effect->randLife.Add.nMax, effect->randLife.Add.nMin);
		}
		fprintf(pFile, "end\n\n");

		// �ړ����x
		fprintf(pFile, "Speed_X\n");
		fprintf(pFile, "nValue = %f\n", effect->speedX.fValue);
		if (effect->speedX.initial.bIsRandom)
		{
			fprintf(pFile, "InitialRand = %f %f\n", effect->speedX.initial.fMax, effect->speedX.initial.fMin);
		}
		if (effect->speedX.Add.bIsRandom)
		{
			fprintf(pFile, "AddRand = %f %f\n", effect->speedX.Add.fMax, effect->speedX.Add.fMin);
		}
		fprintf(pFile, "end\n\n");

		fprintf(pFile, "Speed_Y\n");
		fprintf(pFile, "nValue = %f\n", effect->speedY.fValue);
		if (effect->speedY.initial.bIsRandom)
		{
			fprintf(pFile, "InitialRand = %f %f\n", effect->speedY.initial.fMax, effect->speedY.initial.fMin);
		}
		if (effect->speedY.Add.bIsRandom)
		{
			fprintf(pFile, "AddRand = %f %f\n", effect->speedY.Add.fMax, effect->speedY.Add.fMin);
		}
		fprintf(pFile, "end\n\n");

		fprintf(pFile, "Speed_Z\n");
		fprintf(pFile, "nValue = %f\n", effect->speedZ.fValue);
		if (effect->speedZ.initial.bIsRandom)
		{
			fprintf(pFile, "InitialRand = %f %f\n", effect->speedZ.initial.fMax, effect->speedZ.initial.fMin);
		}
		if (effect->speedZ.Add.bIsRandom)
		{
			fprintf(pFile, "AddRand = %f %f\n", effect->speedZ.Add.fMax, effect->speedZ.Add.fMin);
		}
		fprintf(pFile, "end\n\n");

		// ���ˊp�x
		fprintf(pFile, "Aangle_X\n");
		fprintf(pFile, "nValue = %f\n", effect->shotAngleX.fValue);
		if (effect->shotAngleX.initial.bIsRandom)
		{
			fprintf(pFile, "InitialRand = %f %f\n", effect->shotAngleX.initial.fMax, effect->shotAngleX.initial.fMin);
		}
		if (effect->shotAngleX.Add.bIsRandom)
		{
			fprintf(pFile, "AddRand = %f %f\n", effect->shotAngleX.Add.fMax, effect->shotAngleX.Add.fMin);
		}
		fprintf(pFile, "end\n\n");

		fprintf(pFile, "Aangle_Y\n");
		fprintf(pFile, "nValue = %f\n", effect->shotAngleY.fValue);
		if (effect->shotAngleY.initial.bIsRandom)
		{
			fprintf(pFile, "InitialRand = %f %f\n", effect->shotAngleY.initial.fMax, effect->shotAngleY.initial.fMin);
		}
		if (effect->shotAngleY.Add.bIsRandom)
		{
			fprintf(pFile, "AddRand = %f %f\n", effect->shotAngleY.Add.fMax, effect->shotAngleY.Add.fMin);
		}
		fprintf(pFile, "end\n\n");

		fprintf(pFile, "Aangle_Z\n");
		fprintf(pFile, "nValue = %f\n", effect->shotAngleZ.fValue);
		if (effect->shotAngleZ.initial.bIsRandom)
		{
			fprintf(pFile, "InitialRand = %f %f\n", effect->shotAngleZ.initial.fMax, effect->shotAngleZ.initial.fMin);
		}
		if (effect->shotAngleZ.Add.bIsRandom)
		{
			fprintf(pFile, "AddRand = %f %f\n", effect->shotAngleZ.Add.fMax, effect->shotAngleZ.Add.fMin);
		}
		fprintf(pFile, "end\n\n");

		// �d��
		fprintf(pFile, "gravity\n");
		fprintf(pFile, "nValue = %f\n", effect->fGravity);
		fprintf(pFile, "end\n\n");

		// ������
		fprintf(pFile, "DivisionU\n");
		fprintf(pFile, "nValue = %d\n", effect->nDivisionU);
		fprintf(pFile, "end\n\n");
		// ������
		fprintf(pFile, "DivisionV\n");
		fprintf(pFile, "nValue = %d\n", effect->nDivisionV);
		fprintf(pFile, "end\n\n");

		// �r���{�[�h
		if (effect->bIsBillboard)
		{
			fprintf(pFile, "Billboard\n");
			fprintf(pFile, "end\n\n");
		}

		// �J�����O
		if (effect->bIsCulling)
		{
			fprintf(pFile, "Culling\n");
			fprintf(pFile, "end\n\n");
		}

		// �I��
		fprintf(pFile, "\nFileEnd\n");
		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ��ꍇ
		printf("\n * * * �t�@�C�����J���܂��� * * * \n");
	}
}
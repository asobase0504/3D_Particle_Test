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

		// ���a
		fprintf(pFile, "Radius\n");
		fprintf(pFile, "nValue = %f\n", effect->randRadius.fValue);
		if (effect->randRadius.initial.bIsRandom)
		{
			fprintf(pFile, "InitialRand = %f %f\n", effect->randRadius.initial.fMax, effect->randRadius.initial.fMin);
		}
		if (effect->randRadius.Add.bIsRandom)
		{
			fprintf(pFile, "AddRand = %f %f\n", effect->randRadius.Add.fMax, effect->randRadius.Add.fMin);
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
		fprintf(pFile, "Speed\n");
		fprintf(pFile, "nValue = %f\n", effect->randSpeed.fValue);
		if (effect->randSpeed.initial.bIsRandom)
		{
			fprintf(pFile, "InitialRand = %f %f\n", effect->randSpeed.initial.fMax, effect->randSpeed.initial.fMin);
		}
		if (effect->randSpeed.Add.bIsRandom)
		{
			fprintf(pFile, "AddRand = %f %f\n", effect->randSpeed.Add.fMax, effect->randSpeed.Add.fMin);
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
#include"connectionExtract.h"

void fillRunVectors(const Mat& bwImage, int& NumberOfRuns, vector<int>& stRun, vector<int>& enRun, vector<int>& rowRun)
{
    for (int i = 0; i < bwImage.rows; i++)
    {
        const uchar* rowData = bwImage.ptr<uchar>(i);

        if (rowData[0] > 0)
        {
            NumberOfRuns++;
            stRun.push_back(0);
            rowRun.push_back(i);
        }
        for (int j = 1; j < bwImage.cols; j++)
        {
            if (rowData[j - 1] < 1 && rowData[j] > 0)
            {
                NumberOfRuns++;
                stRun.push_back(j);
                rowRun.push_back(i);
            }
            else if (rowData[j - 1] > 0 && rowData[j] < 1)
            {
                enRun.push_back(j - 1);
            }
        }
        if (rowData[bwImage.cols - 1])
        {
            enRun.push_back(bwImage.cols - 1);
        }
    }
}

void firstPass(vector<int>& stRun, vector<int>& enRun, vector<int>& rowRun, int NumberOfRuns, 
    vector<int>& runLabels, vector<pair<int, int>>& equivalences, int offset) 
{ 
    runLabels.assign(NumberOfRuns, 0); 
    int idxLabel = 1; 
    int curRowIdx = 0; 
    int firstRunOnCur = 0; 
    int firstRunOnPre = 0; 
    int lastRunOnPre = -1; 
    for (int i = 0; i < NumberOfRuns; i++) 
    { 
        // ����Ǹ��еĵ�һ��run���������һ�е�һ��run�����һ��run����� 
        if (rowRun[i] != curRowIdx) 
        { 
            curRowIdx = rowRun[i]; // �����е���� 
            firstRunOnPre = firstRunOnCur; 
            lastRunOnPre = i - 1; 
            firstRunOnCur = i; 
        } 
        // ������һ�е�����run���ж��Ƿ��ڵ�ǰrun���غϵ����� 
        for (int j = firstRunOnPre; j <= lastRunOnPre; j++) 
        { 
            // �����غ� �� �������ڵ����� 
            if (stRun[i] <= enRun[j] + offset && enRun[i] >= stRun[j] - offset && rowRun[i] == rowRun[j] + 1) 
            { 
                if (runLabels[i] == 0) // û�б���Ź� 
                    runLabels[i] = runLabels[j]; 
                else if (runLabels[i] != runLabels[j])// �Ѿ������              
                    equivalences.push_back(make_pair(runLabels[i], runLabels[j])); // ����ȼ۶� 
            } 
        } 
        if (runLabels[i] == 0) // û����ǰһ�е��κ�run�غ� 
        { 
            runLabels[i] = idxLabel++; 
        } 
  
    } 
} 


void replaceSameLabel(vector<int>& runLabels, vector<pair<int, int>>&
    equivalence)
{
    int maxLabel = *max_element(runLabels.begin(), runLabels.end());
    vector<vector<bool>> eqTab(maxLabel, vector<bool>(maxLabel, false));
    vector<pair<int, int>>::iterator vecPairIt = equivalence.begin();
    while (vecPairIt != equivalence.end())
    {
        eqTab[vecPairIt->first - 1][vecPairIt->second - 1] = true;
        eqTab[vecPairIt->second - 1][vecPairIt->first - 1] = true;
        vecPairIt++;
    }
    vector<int> labelFlag(maxLabel, 0);
    vector<vector<int>> equaList;
    vector<int> tempList;
    cout << maxLabel << endl;
    for (int i = 1; i <= maxLabel; i++)
    {
        if (labelFlag[i - 1])
        {
            continue;
        }
        labelFlag[i - 1] = equaList.size() + 1;
        tempList.push_back(i);
        for (vector<int>::size_type j = 0; j < tempList.size(); j++)
        {
            for (vector<bool>::size_type k = 0; k != eqTab[tempList[j] - 1].size(); k++)
            {
                if (eqTab[tempList[j] - 1][k] && !labelFlag[k])
                {
                    tempList.push_back(k + 1);
                    labelFlag[k] = equaList.size() + 1;
                }
            }
        }
        equaList.push_back(tempList);
        tempList.clear();
    }
    cout << equaList.size() << endl;
    for (vector<int>::size_type i = 0; i != runLabels.size(); i++)
    {
        runLabels[i] = labelFlag[runLabels[i] - 1];
    }
}
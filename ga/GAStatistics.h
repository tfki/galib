// $Header$
/* ----------------------------------------------------------------------------
  statistics.h
  mbwall 14jul95
  Copyright (c) 1995 Massachusetts Institute of Technology
				   - all rights reserved

 DESCRIPTION:
  Header for the statistics object used by the GA objects.
---------------------------------------------------------------------------- */
#ifndef _ga_statistics_h_
#define _ga_statistics_h_

#include <GAGenome.h>
#include <GAPopulation.h>
#include <gaconfig.h>
#include <gatypes.h>

// Default settings and their names.
extern int gaDefNumBestGenomes;
extern int gaDefScoreFrequency1;
extern int gaDefScoreFrequency2;
extern int gaDefFlushFrequency;
extern std::string gaDefScoreFilename;

/* ----------------------------------------------------------------------------
Statistics class
  We define this class as a storage object for the current state of the GA.
Whereas the parameters object keeps track of the user-definable settings for
the GA, the statistics object keeps track of the data that the GA generates
along the way.
---------------------------------------------------------------------------- */
class GAStatistics
{
  public:
	enum
	{
		NoScores = 0x00,
		Mean = 0x01,
		Maximum = 0x02,
		Minimum = 0x04,
		Deviation = 0x08,
		Diversity = 0x10,
		AllScores = 0xff
	};

	GAStatistics();
	GAStatistics(const GAStatistics &);
	GAStatistics &operator=(const GAStatistics &orig)
	{
		copy(orig);
		return *this;
	}
	virtual ~GAStatistics();
	void copy(const GAStatistics &);

	float online() const { return on; }
	float offlineMax() const { return offmax; }
	float offlineMin() const { return offmin; }
	float initial(int w = Maximum) const;
	float current(int w = Maximum) const;
	float maxEver() const { return maxever; }
	float minEver() const { return minever; }

	int generation() const { return curgen; }
	unsigned long int selections() const { return numsel; }
	unsigned long int crossovers() const { return numcro; }
	unsigned long int mutations() const { return nummut; }
	unsigned long int replacements() const { return numrep; }
	unsigned long int indEvals() const { return numeval; }
	unsigned long int popEvals() const { return numpeval; }
	float convergence() const;

	int nConvergence() const { return Nconv; }
	int nConvergence(unsigned int);
	int nBestGenomes(const GAGenome &, unsigned int);
	int nBestGenomes() const { return (boa != nullptr ? boa->size() : 0); }
	int scoreFrequency(unsigned int x) { return (scoreFreq = x); }
	int scoreFrequency() const { return scoreFreq; }
	int flushFrequency(unsigned int x);
	int flushFrequency() const { return Nscrs; }
	std::string scoreFilename(const std::string &filename);
	std::string scoreFilename() const { return scorefile; }
	int selectScores(int w) { return which = w; }
	int selectScores() const { return which; }
	bool recordDiversity(bool flag) { return dodiv = flag; }
	bool recordDiversity() const { return dodiv; }
	void flushScores();

	void update(const GAPopulation &pop);
	void reset(const GAPopulation &pop);
	const GAPopulation &bestPopulation() const { return *boa; }
	const GAGenome &bestIndividual(unsigned int n = 0) const;

	int scores(const std::string &filename, int which = NoScores);
	int scores(std::ostream &os, int which = NoScores);
	int write(const std::string &filename) const;
	int write(std::ostream &os) const;

	// These should be protected (accessible only to the GA class) but for now
	// they are publicly accessible.  Do not try to set these unless you know
	// what you are doing!!
	unsigned long int numsel; // number of selections since reset
	unsigned long int numcro; // number of crossovers since reset
	unsigned long int nummut; // number of mutations since reset
	unsigned long int numrep; // number of replacements since reset
	unsigned long int numeval; // number of individual evaluations since reset
	unsigned long int numpeval; // number of population evals since reset

  protected:
	unsigned int curgen; // current generation number
	unsigned int scoreFreq; // how often (in generations) to record scores
	bool dodiv; // should we record diversity?

	float maxever; // maximum score since initialization
	float minever; // minimum score since initialization
	float on; // "on-line" performance (ave of all scores)
	float offmax; // "off-line" performance (ave of maximum)
	float offmin; // "off-line" performance (ave of minimum)

	float aveInit; // stats from the initial population
	float maxInit;
	float minInit;
	float devInit;
	float divInit;

	float aveCur; // stats from the current population
	float maxCur;
	float minCur;
	float devCur;
	float divCur;

	unsigned int nconv, Nconv; // how many scores we're recording (flushFreq)
	float *cscore; // best score of last n generations

	// how many scores do we have?
	unsigned int nscrs, Nscrs; 
	// generation number corresponding to scores
	std::vector<int> gen; 
	// average scores of each generation
	std::vector<float> aveScore; 
	// best scores of each generation
	std::vector<float> maxScore; 
	// worst scores of each generation
	std::vector<float> minScore; 
	// stddev of each generation
	std::vector<float> devScore; 
	// diversity of each generation
	std::vector<float> divScore;
	// name of file to which scores get written
	std::string scorefile; 
	// which data to write to file
	int which; 
	// keep a copy of the best genomes
	GAPopulation *boa; 

	void setConvergence(float);
	void setScore(const GAPopulation &);
	void updateBestIndividual(const GAPopulation &, bool flag = false);
	void writeScores();
	void resizeScores(unsigned int);

	friend class GA;
};

inline std::string GAStatistics::scoreFilename(const std::string &filename)
{
	scorefile = filename;
	return scorefile;
}

inline float GAStatistics::convergence() const
{
	double cnv = 0.0;
	if (nconv >= Nconv - 1 && cscore[nconv % Nconv] != 0) {
		cnv = static_cast<double>(cscore[(nconv + 1) % Nconv]) /
			  static_cast<double>(cscore[nconv % Nconv]);
}
	return static_cast<float>(cnv);
}

inline float GAStatistics::initial(int w) const
{
	float val = 0.0;
	switch (w)
	{
	case Mean:
		val = aveInit;
		break;
	case Maximum:
		val = maxInit;
		break;
	case Minimum:
		val = minInit;
		break;
	case Deviation:
		val = devInit;
		break;
	case Diversity:
		val = divInit;
		break;
	default:
		break;
	}
	return val;
}

inline float GAStatistics::current(int w) const
{
	float val = 0.0;
	switch (w)
	{
	case Mean:
		val = aveCur;
		break;
	case Maximum:
		val = maxCur;
		break;
	case Minimum:
		val = minCur;
		break;
	case Deviation:
		val = devCur;
		break;
	case Diversity:
		val = divCur;
		break;
	default:
		break;
	}
	return val;
}

inline std::ostream &operator<<(std::ostream &os, const GAStatistics &s)
{
	s.write(os);
	return os;
}

#endif

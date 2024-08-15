#include "tournament_round.h"

// TournamentRound functions goes here

TournamentRound::TournamentRound() {}
TournamentRound::TournamentRound(std::list<MusicBand *> _bands)
{
    bands = _bands;
}
TournamentRound::TournamentRound(std::vector<MusicBand *> _bands)
{
    std::list<MusicBand *> liste(_bands.begin(), _bands.end());
    bands = liste;
}

std::size_t TournamentRound::size() { return bands.size(); }

TournamentRound &TournamentRound::operator=(TournamentRound &&other)
{
    if (this != &other)
    {
        bands = std::move(other.bands);
    }
    return *this;
}
TournamentRound &TournamentRound::get_next_round()
{
    std::list<MusicBand *> Winner;

    int score1, score2, fark, f1, f2;
    while (bands.size() > 1)
    {
        auto front = bands.begin();
        auto back = --bands.end();

        score1 = (*front)->play(*back);
        score2 = (*back)->play(*front);
        f1 = (*front)->get_fan_count();
        f2 = (*back)->get_fan_count();
        if (score2 > score1)
        {
            Winner.push_back(*back);
            fark = score2 - score1;
            if (fark > f1)
            {
                (*front)->set_fan_count(0);
                (*back)->set_fan_count(f1 + f2);
            }
            else
            {
                (*front)->set_fan_count(f1 - fark);
                (*back)->set_fan_count(f2 + fark);
            }
        }
        else if (score1 >= score2)
        {
            Winner.push_back(*front);
            fark = score1 - score2;
            if (fark > f2)
            {
                (*front)->set_fan_count(f1 + f2);
                (*back)->set_fan_count(0);
            }
            else
            {
                (*front)->set_fan_count(f1 + fark);
                (*back)->set_fan_count(f2 - fark);
            }
        }
        bands.pop_front();
        bands.pop_back();
    }
    if (bands.size() == 1)
    {
        auto front = bands.begin();
        Winner.push_back(*front);
    }
    bands = Winner;
    return *this;
}

std::ostream &operator<<(std::ostream &os, TournamentRound &other)
{
    for (auto iterator = other.bands.begin(); iterator != other.bands.end(); iterator++)
    {
        if (iterator != other.bands.begin())
        {
            os << "\t";
        }
        os << (*iterator)->get_name();
    }
    return os;
}
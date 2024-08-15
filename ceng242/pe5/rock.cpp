#include "metal.h"
#include "jazz.h"
#include "kpop.h"
#include "rock.h"

int RockBand::play(MusicBand *other)
{
    int score;
    RockBand *rock = dynamic_cast<RockBand *>(other);
    JazzBand *jazz = dynamic_cast<JazzBand *>(other);
    MetalBand *metal = dynamic_cast<MetalBand *>(other);
    KPopBand *kpop = dynamic_cast<KPopBand *>(other);
    if (rock)
    {
        score = ((this->get_fan_count()) + (this->get_talent()) * (this->get_energy()) / 10);
    }
    else if (jazz)
    {
        score = ((this->get_fan_count()) + (this->get_talent()) * (this->get_energy()) / 10) * (0.8);
    }
    else if (metal)
    {
        score = ((this->get_fan_count()) + (this->get_talent()) * (this->get_energy()) / 10) * (1.4);
    }
    else if (kpop)
    {
        score = ((this->get_fan_count()) + (this->get_talent()) * (this->get_energy()) / 10) * (0.5);
    }

    this->set_energy(0.90 * this->get_energy());
    return score;
}

void RockBand::rehearse(void)
{
    this->set_energy(0.95 * this->get_energy());
    this->set_talent(this->get_talent() + 10);
}
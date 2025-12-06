// poison_resistant_racket.hpp
#ifndef __POISON_RESISTANT_RACKET_HPP
#define __POISON_RESISTANT_RACKET_HPP

#include "defense_perks.hpp"

class poison_resistant_racket : public defense_perks {
public:
    poison_resistant_racket() = default;
    ~poison_resistant_racket() = default;
    poison_resistant_racket(bool perk_collected, int perk_time, bool perk_appear, sf::Vector2f& perk_position, int perk_impact, const std::string& perk_path);
    void impact_on_player(player& p) override;
};

#endif  // __POISON_RESISTANT_RACKET_HPP

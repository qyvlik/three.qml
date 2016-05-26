#ifndef THREE_LAYERS_H
#define THREE_LAYERS_H


namespace three {

class Layers
{
public:
    Layers():
        mask(1)
    {

    }

    void set(const int& channel)
    {
        this->mask = 1 << channel;
    }

    void enable(const int& channel)
    {
        this->mask |= 1 << channel;
    }

    void toggle(const int& channel)
    {
        this->mask ^= 1 << channel;
    }

    void disable(const int& channel)
    {
        this->mask &= ~(1 << channel );
    }

    bool test(const Layers& layers) const
    {
        return (this->mask & layers.mask) != 0;
    }

    // private:
    int mask;
};

} // namespace three

#endif // THREE_LAYERS_H

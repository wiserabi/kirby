#pragma once

class SRV : public SingletonBase<SRV>
{
public:
    friend SingletonBase<SRV>;

private:
    SRV();
    ~SRV();

public:
    void AddSRV(string path, ID3D11ShaderResourceView* srv);
    ID3D11ShaderResourceView* GetSRV(string path);

private:
    map<string, ID3D11ShaderResourceView*> srvs;
};
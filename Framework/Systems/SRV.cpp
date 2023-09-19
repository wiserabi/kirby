#include "Framework.h"
#include "SRV.h"

SRV::SRV()
{
}

SRV::~SRV()
{
    for (auto it = srvs.begin(); it != srvs.end(); ++it)
        SAFE_RELEASE(it->second);

    srvs.clear();

    cout << "SRV Manager : srvs clear complete" << endl;
}

void SRV::AddSRV(string path, ID3D11ShaderResourceView* srv)
{
    if (srv == nullptr || path == "")
    {
        cout << "SRV Manager : Add Error" << endl;
        return;
    }

    srvs.insert(make_pair(path, srv));
}

ID3D11ShaderResourceView* SRV::GetSRV(string path)
{
    if (srvs.find(path) != srvs.end())
        return srvs.find(path)->second;
    else
    {
        cout << "SRV Manager : Can't find path" << endl;
        return nullptr;
    }
}

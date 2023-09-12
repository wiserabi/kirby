#include "Framework.h"
#include "VertexBuffer.h"

VertexBuffer::~VertexBuffer()
{
    SAFE_RELEASE(buffer);
}

void VertexBuffer::SetIA()
{
    DC->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
}

// IASetVertexBuffers 매개변수
/*
StartSlot       : 입력 슬롯 인덱스
NumBuffers      : 버퍼 개수
ppVertexBuffers : 버퍼를 가리키는 포인터 변수
pStrides        : 구조체 크기
pOffsets        : 버퍼의 오프셋 값 (특정 위치부터 읽어들이고자 할 때 사용)
*/
/*
	Muli3D - a software rendering library
	Copyright (C) 2004, 2005 Stephan Reiter <streiter@aon.at>

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "../../include/core/m3dcore_indexbuffer.h"
#include "../../include/core/m3dcore_device.h"

CMuli3DIndexBuffer::CMuli3DIndexBuffer( CMuli3DDevice *i_pParent )
	: m_pParent( i_pParent )
{
	m_pParent->AddRef();
}

result CMuli3DIndexBuffer::Create( uint32 i_iLength, m3dformat i_fmtFormat )
{
	if( !i_iLength )
	{
		FUNC_FAILING( "CMuli3DIndexBuffer::Create: parameter i_iLength is 0.\n" );
		return e_invalidparameters;
	}

	if( i_fmtFormat != m3dfmt_index16 && i_fmtFormat != m3dfmt_index32 )
	{
		FUNC_FAILING( "CMuli3DIndexBuffer::Create: invalid format specified.\n" );
		return e_invalidformat;
	}

	m_iLength = i_iLength;
	m_fmtFormat = i_fmtFormat;

	m_pData = new byte[i_iLength];
	if( !m_pData )
	{
		FUNC_FAILING( "CMuli3DIndexBuffer::Create: out of memory, cannot create index buffer.\n" );
		return e_outofmemory;
	}

	return s_ok;
}

CMuli3DIndexBuffer::~CMuli3DIndexBuffer()
{
	SAFE_DELETE_ARRAY( m_pData );

	SAFE_RELEASE( m_pParent );
}

result CMuli3DIndexBuffer::GetPointer( uint32 i_iOffset, void **o_ppData )
{
	if( !o_ppData )
	{
		FUNC_FAILING( "CMuli3DIndexBuffer::GetPointer: parameter o_ppData points to null.\n" );
		return e_invalidparameters;
	}

	if( i_iOffset >= m_iLength )
	{
		*o_ppData = 0;
		FUNC_FAILING( "CMuli3DIndexBuffer::GetPointer: i_iOffset is larger than index buffer length.\n" );
		return e_invalidparameters;
	}

	*o_ppData = &m_pData[i_iOffset];
	return s_ok;
}

result CMuli3DIndexBuffer::GetVertexIndex( uint32 i_iArrayIndex, uint32 &o_iValue )
{
	switch( m_fmtFormat )
	{
	case m3dfmt_index16:
		{
			if( i_iArrayIndex >= m_iLength / 2 )
			{
				FUNC_FAILING( "CMuli3DIndexBuffer::GetVertexIndex: parameter i_iArrayIndex exceeds vertex buffer size.\n" );
				return e_invalidparameters;
			}

			const uint16 *pData = (const uint16 *)m_pData;
			o_iValue = pData[i_iArrayIndex];
			return s_ok;
		}
	case m3dfmt_index32:
		{
			if( i_iArrayIndex >= m_iLength / 4 )
			{
				FUNC_FAILING( "CMuli3DIndexBuffer::GetVertexIndex: parameter i_iArrayIndex exceeds vertex buffer size.\n" );
				return e_invalidparameters;
			}

			const uint32 *pData = (const uint32 *)m_pData;
			o_iValue = pData[i_iArrayIndex];
			return s_ok;
		}
	default:
		// cannot happen
		FUNC_FAILING( "CMuli3DIndexBuffer::GetVertexIndex: invalid format.\n" );
		return e_invalidformat;
	}
}

CMuli3DDevice *CMuli3DIndexBuffer::pGetDevice()
{
	if( m_pParent )
		m_pParent->AddRef();
	return m_pParent;
}

uint32 CMuli3DIndexBuffer::iGetLength()
{
	return m_iLength;
}

m3dformat CMuli3DIndexBuffer::fmtGetFormat()
{
	return m_fmtFormat;
}
